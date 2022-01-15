#include "pch.h"
#include "d3dhook.h"
#include "menuwindow.h"
#include "menuinfo.h"

void MainThread(void* param)
{
    gLog << "Starting...\nVersion: " MENU_TITLE "\nAuthor: Grinch_\nDiscord: " DISCORD_INVITE "\nMore Info: "
        GITHUB_LINK "\n" << std::endl;

    D3dHook::InjectHook(MenuWindow::Draw);

    while (true)
    {
        MenuWindow::Process();
        Sleep(250); // wait so we don't overload the CPU
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&MainThread, nullptr, NULL, nullptr);
    }

    return TRUE;
}

