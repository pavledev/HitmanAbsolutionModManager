#pragma once

#include "imgui.h"

class UI
{
public:
	static bool IconButton(const char* icon, const char* label, ImVec4 iconColor = { 1.f, 1.f, 1.f, 1.0f }, ImVec4 backgroundColor = { 0.f, 0.501f, 1.f, 1.f });
};
