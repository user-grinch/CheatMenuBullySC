#pragma once
#include "pch.h"

using f_EndScene = HRESULT(CALLBACK*)(IDirect3DDevice9*);
using f_Reset = HRESULT(CALLBACK*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using f_GetDeviceState = HRESULT(CALLBACK*)(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData);
using f_GetDeviceData = HRESULT(CALLBACK*)(DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD);

class D3dHook
{
private:
	static inline f_EndScene oEndScene = nullptr;
	static inline f_Reset oReset = nullptr;
	static inline f_GetDeviceState oGetDeviceState = nullptr;
	static inline f_GetDeviceData oGetDeviceData = nullptr;
	static inline void* pCallbackFunc = nullptr;

	static bool GetD3D9Device(void** pTable, size_t Size);
	static bool GetDinputDevice(void** pMouse, size_t Size);
	static HRESULT CALLBACK hkEndScene(IDirect3DDevice9* ptr);
	static HRESULT CALLBACK hkGetDeviceState(IDirectInputDevice8* pThis, DWORD cbData, LPVOID lpvData);
	static HRESULT CALLBACK hkReset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

public:

	D3dHook() = delete;
	D3dHook(D3dHook const&) = delete;
	void operator=(D3dHook const&) = delete;
	static void InjectHook(void *pCallback);
	static void RemoveHook();
};
