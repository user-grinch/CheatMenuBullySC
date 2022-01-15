#include "pch.h"
#include "ui.h"
#include "menuWindow.h"

bool Ui::ListBox(const char* label, std::vector<std::string>& all_items, int& selected)
{
	bool rtn = false;
	if (ImGui::BeginCombo(label, all_items[selected].c_str()))
	{
		for (size_t index = 0; index < all_items.size(); index++)
		{
			if (selected != index)
			{
				if (ImGui::MenuItem(all_items[index].c_str()))
				{
					selected = index;
					rtn = true;
				}
			}
		}
		ImGui::EndCombo();
	}
	return rtn;
}

bool Ui::ListBoxStr(const char* label, std::vector<std::string>& all_items, std::string& selected)
{
	bool rtn = false;
	if (ImGui::BeginCombo(label, selected.c_str()))
	{
		for (std::string current_item : all_items)
		{
			if (ImGui::MenuItem(current_item.c_str()))
			{
				selected = current_item;
				rtn = true;
			}
		}
		ImGui::EndCombo();
	}

	return rtn;
}

void Ui::DrawJSON(JsonData& data, JsonCallback func_left_click, JsonCallback func_right_click)
{
	ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() / 2 - 5);
	ListBoxStr("##Categories", data.m_Categories, data.m_Selected);
	ImGui::SameLine();

	data.m_Filter.Draw("##Filter");
	if (strlen(data.m_Filter.InputBuf) == 0)
	{
		ImDrawList* drawlist = ImGui::GetWindowDrawList();

		ImVec2 min = ImGui::GetItemRectMin();
		min.x += ImGui::GetStyle().FramePadding.x;
		min.y += ImGui::GetStyle().FramePadding.y;

		drawlist->AddText(min, ImGui::GetColorU32(ImGuiCol_TextDisabled), "Search");
	}

	ImGui::PopItemWidth();

	ImGui::Spacing();

	if (ImGui::IsMouseClicked(1))
	{
		jsonPopup.function = nullptr;
	}


	ImGui::BeginChild(1);
	for (auto root : data.m_pJson->m_Data.items())
	{
		if (root.key() == data.m_Selected || data.m_Selected == "All")
		{
			for (auto _data : root.value().items())
			{
				std::string name = _data.key();
				if (data.m_Filter.PassFilter(name.c_str()))
				{
					if (ImGui::MenuItem(name.c_str()) && func_left_click != nullptr)
					{
						std::string root_key = root.key();
						std::string data_key = _data.key();
						std::string data_val = _data.value();

						func_left_click(root_key, data_key, data_val);
					}

					if (ImGui::IsItemClicked(1) && func_right_click != nullptr)
					{
						jsonPopup.function = func_right_click;
						jsonPopup.rootKey = root.key();
						jsonPopup.key = name;
						jsonPopup.value = _data.value();
					}
				}
			}
		}
	}

	if (jsonPopup.function != nullptr)
	{
		if (ImGui::BeginPopupContextWindow())
		{
			ImGui::Text(jsonPopup.key.c_str());
			ImGui::Separator();
			if (ImGui::MenuItem("Remove"))
			{
				jsonPopup.function(jsonPopup.rootKey, jsonPopup.key, jsonPopup.value);
			}


			if (ImGui::MenuItem("Close"))
			{
				jsonPopup.function = nullptr;
			}

			ImGui::EndPopup();
		}
	}
	ImGui::EndChild();
}

ImVec2 Ui::GetSize(short count, bool spacing)
{
	if (count == 1)
	{
		spacing = false;
	}

	float factor = ImGui::GetStyle().ItemSpacing.x / 2.0f;
	float x;

	if (count == 3)
	{
		factor = ImGui::GetStyle().ItemSpacing.x / 1.403f;
	}

	if (spacing)
	{
		x = ImGui::GetWindowContentRegionWidth() / count - factor;
	}
	else
	{
		x = ImGui::GetWindowContentRegionWidth() / count;
	}

	return ImVec2(x, ImGui::GetFrameHeight() * 1.3f);
}

void Ui::CenterdText(const std::string& text)
{
	ImVec2 size = ImGui::CalcTextSize(text.c_str());
	ImGui::NewLine();
	ImGui::SameLine(
		((ImGui::GetWindowContentRegionWidth() - size.x) / 2)
	);

	ImGui::Text(text.c_str());
}

void Ui::DrawHeaders(CallbackTable& data)
{
	static void* pCallback;
	static int buttonInRow = 3;
	ImVec2 size = GetSize(buttonInRow, false);
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 buttonCol = style.Colors[ImGuiCol_Button];

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	for (size_t i = 0; i < data.size(); ++i)
	{
		const char* btn_text = data[i].first.c_str();

		if (btn_text == MenuWindow::m_HeaderId)
		{
			style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_ButtonActive];
			pCallback = data[i].second;
		}
		if (ImGui::Button(btn_text, size))
		{
			MenuWindow::m_HeaderId = btn_text;
			pCallback = data[i].second;
		}
		style.Colors[ImGuiCol_Button] = buttonCol;

		if (i % buttonInRow != 2)
		{
			ImGui::SameLine();
		}
	}
	ImGui::PopStyleVar();
	ImGui::Dummy(ImVec2(0, 20));

	if (MenuWindow::m_HeaderId == "")
	{
		// Show Welcome page
		ImGui::NewLine();

		CenterdText("Welcome to Cheat Menu");
		CenterdText("Author: Grinch_");

		ImGui::NewLine();
		ImGui::TextWrapped("Please ensure you have the latest version from GitHub.");
		ImGui::NewLine();
		if (ImGui::Button("Discord server", ImVec2(GetSize(2))))
		{
			ShellExecuteA(nullptr, "open", DISCORD_INVITE, nullptr, nullptr, SW_SHOWNORMAL);
		}

		ImGui::SameLine();

		if (ImGui::Button("GitHub repo", ImVec2(GetSize(2))))
		{
			ShellExecuteA(nullptr, "open", GITHUB_LINK, nullptr, nullptr, SW_SHOWNORMAL);
		}

		ImGui::NewLine();
		ImGui::TextWrapped("If you find bugs or have suggestions, you can let me know on discord :)");
		ImGui::Dummy(ImVec2(0, 15));
		CenterdText("Copyright Grinch_ 2021-2022. All rights reserved.");
	}
	else
	{
		if (pCallback != nullptr && ImGui::BeginChild("TABSBAR"))
		{
			static_cast<void(*)()>(pCallback)();
			ImGui::EndChild();
		}
	}
}
