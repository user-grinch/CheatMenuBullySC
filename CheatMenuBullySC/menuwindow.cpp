#include "pch.h"
#include "menuwindow.h"
#include "menuinfo.h"
#include "ui.h"
#include "overlaywindow.h"
#include "notifiypopup.h"
#include "vendor/patch/assembly.hpp"

void MenuWindow::Draw()
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = m_bShowMenu;

    static bool bInit;
    // Set a proper size
    if (!bInit)
    {
        OverlayWindow::Init();
        ApplyStyle();
        bInit = true;
    }

    if (m_bShowMenu)
    {
        static bool bInit2;
        // Set a proper size
        if (!bInit2)
        {
            RECT rect;
            LONG width, height;
            if (GetWindowRect(GetForegroundWindow(), &rect))
            {
                width = (rect.right - rect.left);
                height = (rect.bottom - rect.top);
            }

            m_HeaderId = gConfig.GetValue("window.id", std::string(""));
            m_fMenuSize.x = gConfig.GetValue("window.sizeX", width / 4.0f);
            m_fMenuSize.y = gConfig.GetValue("window.sizeY", height / 1.2f);
            ImGui::SetNextWindowSize(m_fMenuSize);

            bInit2 = true;
        }

        ImGui::Begin(MENU_TITLE, &m_bShowMenu, ImGuiWindowFlags_NoCollapse);
        Ui::DrawHeaders(header);

        m_fMenuSize = ImGui::GetWindowSize();
        gConfig.SetValue("window.sizeX", m_fMenuSize.x);
        gConfig.SetValue("window.sizeY", m_fMenuSize.y);
        ImGui::End();
    }

    OverlayWindow::Draw();
    NotifiyPopup::Draw();
}

void MenuWindow::Process()
{
    // TODO: Implement proper hotkeys
    ImGuiIO& io = ImGui::GetIO();
    if (GetKeyState(VK_CONTROL) & 0x8000)
    {
        io.KeyCtrl = true;
        if ((GetKeyState(VK_KEY_M) & 0x8000) != 0)
        {
            m_bShowMenu = !m_bShowMenu;
            Sleep(250);
        }
    }
    else
    {
        io.KeyCtrl = false;
    }

    if (GetKeyState(VK_SHIFT) & 0x8000)
    {
        io.KeyShift = true;
    }
    else
    {
        io.KeyShift = false;
    }

    static bool menuState;
    if (m_bShowMenu != menuState)
    {
        gConfig.WriteToDisk();
        menuState = m_bShowMenu;
    }
}

void MenuWindow::TeleportTab()
{
	if (ImGui::BeginTabBar("Teleport", ImGuiTabBarFlags_NoTooltip + ImGuiTabBarFlags_FittingPolicyScroll))
	{
		ImGui::Spacing();
		if (ImGui::BeginTabItem("Teleport"))
		{
			ImGui::Spacing();
			if (ImGui::BeginChild("Teleport Child"))
			{
				ImGui::Spacing();
				ImGui::InputTextWithHint("Coordinates", "x, y, z", m_nInputBuffer, IM_ARRAYSIZE(m_nInputBuffer));
				ImGui::Spacing();

				if (ImGui::Button("Teleport to coord", Ui::GetSize(2)))
				{
					try
					{
                     /*   CVector pos;
						sscanf(m_nInputBuffer, "%f,%f,%f", &pos.x, &pos.y, &pos.z);
						pos.z += 1.0f;
                        CWorld::GetInstance()->pPlayer->Position = pos;*/
                        NotifiyPopup::AddToQueue("Teleported to coordinate");
					}
					catch (...)
					{
                        NotifiyPopup::AddToQueue("Invalid coordinate");
					}
				}
				ImGui::SameLine();

				if (ImGui::Button("Insert Coordinates", Ui::GetSize(2)))
				{
                   /* CVector pos = CWorld::GetInstance()->pPlayer->Position;

                    strcpy(m_nInputBuffer,
                        (std::to_string(static_cast<int>(pos.x)) + ", " + std::to_string(static_cast<int>(pos.y)) +
                            ", " + std::to_string(static_cast<int>(pos.z))).c_str());*/
				}

				ImGui::EndChild();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Search"))
		{
			ImGui::Spacing();
            Ui::DrawJSON(m_tpData, 
                [](std::string& rootkey, std::string& bLocName, std::string& loc)
                {
                    try
                    {
                        //CVector &pos = CWorld::GetInstance()->pPlayer->Position;
                        //sscanf(loc.c_str(), "%f,%f,%f", &pos.x, &pos.y, &pos.z);
                    }
                    catch (...)
                    {
                        NotifiyPopup::AddToQueue("Invalid location");
                    }
                }, 
                [](std::string& cat, std::string& key, std::string& val)
                {
                    if (cat == "Custom")
                    {
                        m_tpData.m_pJson->m_Data["Custom"].erase(key);
                        NotifiyPopup::AddToQueue("Location removed");
                        m_tpData.m_pJson->WriteToDisk();
                    }
                    else
                    {
                        NotifiyPopup::AddToQueue("You can only remove custom location");
                    }
                });
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Custom"))
		{
			ImGui::Spacing();
			ImGui::InputTextWithHint("Location", "Groove Street", m_nLocationBuffer, IM_ARRAYSIZE(m_nInputBuffer));
			ImGui::InputTextWithHint("Coordinates", "x, y, z", m_nInputBuffer, IM_ARRAYSIZE(m_nInputBuffer));
			ImGui::Spacing();
			if (ImGui::Button("Add location", Ui::GetSize()))
			{
				m_tpData.m_pJson->m_Data["Custom"][m_nLocationBuffer] = std::string(m_nInputBuffer);
				m_tpData.m_pJson->WriteToDisk();
                NotifiyPopup::AddToQueue("Location added");
			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void MenuWindow::ApplyStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->WindowRounding = 1;
    style->ScrollbarRounding = 1;
    style->GrabRounding = 1;
    style->WindowRounding = 1;
    style->ChildRounding = 1;
    style->ScrollbarRounding = 1;
    style->GrabRounding = 1;
    style->FrameRounding = 0;
    style->TabRounding = 1.0;
    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;
    style->Alpha = 1;

    style->FrameBorderSize = 0;
    style->ChildBorderSize = 0;
    style->TabBorderSize = 0;
    style->WindowBorderSize = 0;
    style->PopupBorderSize = 0;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.0f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
}

inline void _declspec(naked) godModePatch()
{
    _asm 
    {
        cmp ecx, [0xC1AEA8] 
        jne func
        ret 4

        func:
     /*   func:
            push ebx
            mov ebx, ecx
            fld dword ptr[ebx + 0x1CB8]
            retn*/
    }
}

void MenuWindow::CheatsTab()
{
    static bool fasterClock;
    if (ImGui::Checkbox("Faster game clock", &fasterClock))
    {
        if (fasterClock)
        {
            injector::MakeInline<0x426135, 0x42613B>([](injector::reg_pack& regs)
            {
                *(uint32_t*)(0xBCEB4A) += 0x10;
            });
        }
        else
        {
            injector::WriteMemoryRaw(0x426135, (void*)"\x88\x15\x4A\xEB\xBC\x00", 6, true);
        }
    }
    static bool godMode;
    if (ImGui::Checkbox("God mode", &godMode))
    {
        if (godMode)
        {
            injector::MakeCALL(0x4783F0, &godModePatch);
            injector::WriteMemoryRaw(0x4783F5, (void*)"\xC2\x04\x00\x90", 4, true);
        }
        else
        {
            injector::WriteMemoryRaw(0x53CFEC, (void*)"\xFF\xD2", 2, true);
        }
    }
    static bool hideHud;
    if (ImGui::Checkbox("Hide hud", &hideHud))
    {
        if (hideHud)
        {
            injector::WriteMemoryRaw(0x53CFEC, (void*)"\x90\x90", 2, true);
        }
        else
        {
            injector::WriteMemoryRaw(0x53CFEC, (void*)"\xFF\xD2", 2, true);
        }
    }
    static bool unlimitedAmmo;
    if (ImGui::Checkbox("Unlimited ammo", &unlimitedAmmo))
    {
        if (unlimitedAmmo)
        {
            injector::MakeInline<0x45CA3A, 0x45CA43>([](injector::reg_pack& regs)
            {
                *(uint16_t*)(regs.esi) = 0x30;
            });
        }
        else
        {
            injector::WriteMemoryRaw(0x45CA3A, (void*)"\x66\x89\x06\x0F\x84\x83\x00\x00\x00", 9, true);
        }
    }
}

void MenuWindow::MenuTab()
{
    if (ImGui::BeginTabBar("MenuTabBar"))
    {
        if (ImGui::BeginTabItem("Overlay"))
        {
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::SameLine();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() / 2);
            if (Ui::ListBox("Overlay", OverlayWindow::posNames, (int&)OverlayWindow::mSelectedPos))
            {
                gConfig.SetValue("overlay.selected_pos", OverlayWindow::mSelectedPos);
            }

            ImGui::Spacing();

            ImGui::Columns(2, nullptr, false);
            if (ImGui::Checkbox("No background", &OverlayWindow::bTransparent))
            {
                gConfig.SetValue("overlay.transparent", OverlayWindow::bTransparent);
            }

            if (ImGui::Checkbox("Show coordinates", &OverlayWindow::bCoord))
            {
                gConfig.SetValue("overlay.coord", OverlayWindow::bCoord);
            }

            if (ImGui::Checkbox("Show CPU usage", &OverlayWindow::bCpuUsage))
            {
                gConfig.SetValue("overlay.cpu_usage", OverlayWindow::bCpuUsage);
            }

            ImGui::NextColumn();

            if (ImGui::Checkbox("Show FPS", &OverlayWindow::bFPS))
            {
                gConfig.SetValue("overlay.fps", OverlayWindow::bFPS);
            }

            if (ImGui::Checkbox("Show RAM usage", &OverlayWindow::bMemUsage))
            {
                gConfig.SetValue("overlay.mem_usage", OverlayWindow::bMemUsage);
            }
            ImGui::Columns(1);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("About"))
        {
            ImGui::Spacing();
            if (ImGui::Button("Check update", ImVec2(Ui::GetSize(3))))
            {
                ShellExecute(NULL, "open", "https://github.com/user-grinch/Cheat-Menu/releases/", NULL, NULL, SW_SHOWNORMAL);
            }

            ImGui::SameLine();

            if (ImGui::Button("Discord server", ImVec2(Ui::GetSize(3))))
            {
                ShellExecuteA(nullptr, "open", DISCORD_INVITE, nullptr, nullptr, SW_SHOWNORMAL);
            }

            ImGui::SameLine();

            if (ImGui::Button("GitHub repo", ImVec2(Ui::GetSize(3))))
            {
                ShellExecuteA(nullptr, "open", GITHUB_LINK, nullptr, nullptr, SW_SHOWNORMAL);
            }
            ImGui::Spacing();

            if (ImGui::BeginChild("AboutChild"))
            {
                ImGui::Columns(2, nullptr, false);
                ImGui::Text("Author: Grinch_");

                ImGui::Text("Version: %s", MENU_VERSION_NUMBER);

                ImGui::NextColumn();
                ImGui::Text("ImGui: %s", ImGui::GetVersion());
                ImGui::Text("Build: %s", BUILD_NUMBER);

                ImGui::Columns(1);

                ImGui::Dummy(ImVec2(0, 10));
                ImGui::TextWrapped("Thanks to Darkpassanger123 & l0wb1t for sharing their work.");
                ImGui::Dummy(ImVec2(0, 10));
                ImGui::TextWrapped("If you find bugs or have suggestions, let me know on discord.");
                ImGui::Dummy(ImVec2(0, 20));
                Ui::CenterdText("Copyright Grinch_ 2021-2022.");

                ImGui::EndChild();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    
}
