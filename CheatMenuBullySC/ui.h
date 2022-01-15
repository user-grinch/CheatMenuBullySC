#pragma once
#include "pch.h"

class Ui
{
private:
	using JsonCallback = std::function<void(std::string&, std::string&, std::string&)>;

	struct JsonPopUpData
	{
		std::function<void(std::string&, std::string&, std::string&)> function;
		std::string key;
		std::string rootKey;
		std::string value;
	};

	static inline JsonPopUpData jsonPopup;

public:
	Ui() = delete;
	Ui(Ui&) = delete;
	void operator=(Ui const&) = delete;

	static void CenterdText(const std::string& text);
	static void DrawHeaders(CallbackTable& data);
	static void DrawJSON(JsonData& data, JsonCallback func_left_click, JsonCallback func_right_click);
	static ImVec2 GetSize(short count = 1, bool spacing = true);
	static bool ListBox(const char* label, std::vector<std::string>& all_items, int& selected);
	static bool ListBoxStr(const char* label, std::vector<std::string>& all_items, std::string& selected);
};

