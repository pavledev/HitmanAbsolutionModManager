#include <format>

#include <Utility/UI.h>
#include <Utility/StringUtility.h>

bool UI::IconButton(const char* icon, const char* label, ImVec4 iconColor, ImVec4 backgroundColor)
{
	const std::string icon2 = std::format("    {}", icon);
	float lineHeight = ImGui::GetTextLineHeight();
	ImVec2 padding = ImGui::GetStyle().FramePadding;

	float width = ImGui::CalcTextSize(icon2.c_str()).x;
	width += ImGui::CalcTextSize(label).x;
	width += padding.x * 2.0f;

	ImGui::PushStyleColor(ImGuiCol_Button, backgroundColor);

	const float cursorPosX = ImGui::GetCursorPosX();
	bool clicked = ImGui::Button(std::format("##{}", label).c_str(), { width, lineHeight + padding.y * 2.0f });

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

	ImGui::SameLine();
	ImGui::SetCursorPosX(cursorPosX);
	ImGui::TextColored(iconColor, "%s", icon2.c_str());
	ImGui::SameLine();
	ImGui::TextUnformatted(label);

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();

	return clicked;
}
