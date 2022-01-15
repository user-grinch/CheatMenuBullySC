#include "pch.h"
#include "d3dhook.h"

HRESULT CALLBACK D3dHook::hkGetDeviceState(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData)
{
	HRESULT result = oGetDeviceState(pThis, cbData, lpvData);

	/*
	* We're detecting it here since usual WndProc doesn't seem to work
	* TODO: This still doesn't detect special keys like Ctrl, Shift etc..
	*/
	if (result == DI_OK && ImGui::GetIO().MouseDrawCursor)
	{
		int frameCount = ImGui::GetFrameCount();
		if (cbData == 16) // mouse
		{
			LPDIMOUSESTATE2 mouseState = reinterpret_cast<LPDIMOUSESTATE2>(lpvData);

			// Block camera rotation
			mouseState->lX = 0;
			mouseState->lY = 0;
			mouseState->lZ = 0;

			static int mouseCount = -1;
			if (frameCount != mouseCount)
			{
				ImGuiIO& io = ImGui::GetIO();
				io.MouseDown[0] = (mouseState->rgbButtons[0] != 0);
				io.MouseDown[1] = (mouseState->rgbButtons[1] != 0);
				mouseCount = frameCount;
			}

			// Block left & right clicks
			mouseState->rgbButtons[0] = 0;
			mouseState->rgbButtons[1] = 0;
		}
		else if (cbData == 256) // keyboard
		{
			static int keyCount = -1;
			if (frameCount != keyCount)
			{
				ImGuiIO& io = ImGui::GetIO();
				for (size_t i = 0; i < cbData; ++i)
				{
					bool state = reinterpret_cast<char*>(lpvData)[i] & 0x80;
					UINT vk = MapVirtualKeyEx(i, MAPVK_VSC_TO_VK, GetKeyboardLayout(NULL));

					// ignore keypresses if holding
					if (io.KeysDown[vk] && state) 
					{
						continue;
					}

					io.KeysDown[vk] = state;
					if (state)
					{
						WCHAR c;
						BYTE keystate[256];
						memset(keystate, 0, 256);
						ToUnicode(vk, i, keystate, &c, 1, 0);

						// Capital letters on shift hold
						if (io.KeyShift && c >= 0x61 && c <= 0x7A)
						{
							c -= 0x20; 
						}
						io.AddInputCharacterUTF16(c);
					}

					if (io.WantTextInput)
					{
						reinterpret_cast<char*>(lpvData)[i] &= ~0x80;
					}
				}

				keyCount = frameCount;
			}
		}
	}
	return result;
}

HRESULT D3dHook::hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	return oReset(pDevice, pPresentationParameters);
}

HRESULT CALLBACK D3dHook::hkEndScene(IDirect3DDevice9* pDevice)
{
	ImGuiIO& io = ImGui::GetIO();
	static bool bInit;
	if (bInit)
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplDX9_NewFrame();

		ImGui::NewFrame();

		if (pCallbackFunc != nullptr)
		{
			((void(*)())pCallbackFunc)();
		}

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	else
	{
		bInit = true;
		
		ImGui_ImplWin32_Init(GetForegroundWindow());
		ImGui_ImplDX9_Init((IDirect3DDevice9*)pDevice);
		ImGui_ImplWin32_EnableDpiAwareness();

		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/trebucbd.ttf", 14.0f);
		io.Fonts->Build();

		ImGuiStyle &style = ImGui::GetStyle();
		style.WindowTitleAlign = ImVec2(0.5, 0.5);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // hide the grip
	}

	return oEndScene(pDevice);
}

bool D3dHook::GetD3D9Device(void** pTable, size_t Size)
{
	if (!pTable)
	{
		return false;
	}

	IDirect3D9* pDevice = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pDevice)
	{
		return false;
	}

	IDirect3DDevice9* pDummyDevice = nullptr;

	// Create dummy device
	D3DPRESENT_PARAMETERS d3dpp{};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetForegroundWindow();

	HRESULT dummyDeviceCreated = pDevice->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDeviceCreated != S_OK)
	{
		// may fail in windowed fullscreen mode, trying again with windowed mode
		d3dpp.Windowed = true;
		dummyDeviceCreated = pDevice->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDeviceCreated != S_OK)
		{
			pDevice->Release();
			return false;
		}
	}

	memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), Size);
	pDummyDevice->Release();
	pDevice->Release();
	return true;
}

bool D3dHook::GetDinputDevice(void** pMouse, size_t Size)
{
	if (!pMouse)
	{
		return false;
	}

	IDirectInput8* pDirectInput = NULL;

	// Create dummy device
	if (DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&pDirectInput, NULL) != DI_OK)
	{
		return false;
	}

	LPDIRECTINPUTDEVICE8  lpdiInput;

	/*
	* We're creating a sysMouse but it still seems to receive keyboard messages?
	*/
	if (pDirectInput->CreateDevice(GUID_SysMouse, &lpdiInput, NULL) != DI_OK)
	{
		pDirectInput->Release();
		return false;
	}

	lpdiInput->SetDataFormat(&c_dfDIKeyboard);
	lpdiInput->SetCooperativeLevel(GetActiveWindow(), DISCL_NONEXCLUSIVE);
	memcpy(pMouse, *reinterpret_cast<void***>(lpdiInput), Size);
	lpdiInput->Release();
	pDirectInput->Release();
	return true;
}

void D3dHook::InjectHook(void* pCallback)
{
	ImGui::CreateContext();

	static void *d3d9Device[119], *diMouse[32];
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)) 
		&& GetDinputDevice(diMouse, sizeof(diMouse))
	)
	{
		pCallbackFunc = pCallback;
		oReset = (f_Reset)DetourFunction((PBYTE)d3d9Device[16], (PBYTE)hkReset);
		oEndScene = (f_EndScene)DetourFunction((PBYTE)d3d9Device[42], (PBYTE)hkEndScene);
		oGetDeviceState = (f_GetDeviceState)DetourFunction((PBYTE)diMouse[9], (PBYTE)hkGetDeviceState);
	}
}

void D3dHook::RemoveHook()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
