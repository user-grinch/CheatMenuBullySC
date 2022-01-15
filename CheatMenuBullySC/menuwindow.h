#pragma once
#include "pch.h"

class MenuWindow
{
private:
	static inline bool m_bShowMenu;

	static inline HANDLE handle = GetModuleHandle(NULL);
	static inline ImVec2 m_fMenuSize;

	// teleport tab
	static inline JsonData m_tpData = JsonData("teleport");
	static inline char m_nInputBuffer[INPUT_BUFFER_SIZE], m_nLocationBuffer[INPUT_BUFFER_SIZE];;
public:
	static inline std::string m_HeaderId;

	MenuWindow() = delete;
	MenuWindow(MenuWindow&) = delete;
	void operator=(MenuWindow const&) = delete;

	// Core
	static void Draw();
	static void Process();

private:
	// Tabs
	static void MenuTab();
	static void CheatsTab();
	static void TeleportTab();
	static void ApplyStyle();

	static inline CallbackTable header{ {"Teleport", TeleportTab}, {"Cheats",&CheatsTab},   {"Menu", &MenuTab}};
};

