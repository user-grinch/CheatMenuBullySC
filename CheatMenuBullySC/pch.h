#pragma once
#define DIRECTINPUT_VERSION 0x0800
#define DISCORD_INVITE "https://discord.gg/ZzW7kmf"
#define GITHUB_LINK "https://github.com/user-grinch/Cheat-Menu-BullySC"
#define INPUT_BUFFER_SIZE 64

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>

#include <d3d9.h>
#include <dinput.h>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_dx9.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/patch/injector.hpp"
#include "json.h"
#include "vkeys.h"
#include "vendor/detours.h"
#include "game/CVector.h"

using CallbackTable = std::vector<std::pair<std::string, void(*)()>>;

struct JsonData
{
	ImGuiTextFilter m_Filter = "";
	std::vector<std::string> m_Categories = { "All" };
	std::string m_Selected = "All";
	std::unique_ptr<CJson> m_pJson;

	JsonData(const char* text)
	{
		m_pJson = std::make_unique<CJson>(text);

		// Generate categories
		for (auto element : m_pJson->m_Data.items())
		{
			m_Categories.push_back(element.key());
		}
	}
};

extern CVector& playerPos;

static std::ofstream gLog = std::ofstream("CheatMenu.log");
// why doesn't this work?
// inline CJson gConfig = CJson("config");
extern CJson gConfig;