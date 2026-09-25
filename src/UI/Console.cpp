#include <format>

#include <IconsMaterialDesignIcons.h>

#include <UI/Console.h>
#include <ModManager.h>
#include <Utility/UI.h>
#include <Utility/StringUtility.h>

Console::Console()
{
	errorIconColor = { 245, 89, 99, 255 };
	warningIconColor = { 254, 228, 160, 255 };
	infoIconColor = { 97, 177, 254, 255 };
	autoScrollIconColor = { 0.537f, 0.753f, 0.286f, 1.0f };
	backgroundColor = { 0.22f, 0.22f, 0.22f, 0.00f };

	showErrors = true;
	showWarnings = true;
	showInfo = true;
	autoScroll = true;

	showErrorButtonBorder = true;
	showWarningButtonBorder = true;
	showInfoButtonBorder = true;
	showAutoScrollButtonBorder = true;

	errorButtonClicked = false;
	warningButtonClicked = false;
	infoButtonClicked = false;
	autoScrollButtonClicked = false;
}

void Console::Render()
{
	RenderButtons();

	ImGui::Spacing();

	RenderTable();
}

void Console::RenderButtons()
{
	unsigned int errorMessageCount = 0;
	unsigned int warningMessageCount = 0;
	unsigned int infoMessageCount = 0;

	for (size_t i = 0; i < logLines.size(); ++i)
	{
		if (logLines[i].level == spdlog::level::level_enum::err)
		{
			++errorMessageCount;
		}
		else if (logLines[i].level == spdlog::level::level_enum::warn)
		{
			++warningMessageCount;
		}
		else if (logLines[i].level == spdlog::level::level_enum::info)
		{
			++infoMessageCount;
		}
	}

	std::string errorButtonText = std::format(" {} Error ", errorMessageCount);
	std::string warningButtonText = std::format(" {} Warning ", warningMessageCount);
	std::string infoButtonText = std::format(" {} Message ", infoMessageCount);

	if (errorMessageCount != 1)
	{
		errorButtonText.insert(errorButtonText.length() - 1, "s ");
	}

	if (warningMessageCount != 1)
	{
		warningButtonText.insert(warningButtonText.length() - 1, "s ");
	}

	if (infoMessageCount != 1)
	{
		infoButtonText.insert(infoButtonText.length() - 1, "s ");
	}

	if (!showErrorButtonBorder)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
	}

	if (UI::IconButton("  " ICON_MDI_CLOSE_CIRCLE, errorButtonText.c_str(), errorIconColor, backgroundColor))
	{
		errorButtonClicked = true;
		filterDirty = true;
	}

	if (!showErrorButtonBorder)
	{
		ImGui::PopStyleVar(1);
	}

	if (errorButtonClicked)
	{
		showErrors = !showErrors;
		showErrorButtonBorder = !showErrorButtonBorder;
		errorButtonClicked = false;
	}

	ImGui::SameLine();
	ImGui::Text("");
	ImGui::SameLine();

	if (!showWarningButtonBorder)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
	}

	if (UI::IconButton("  " ICON_MDI_ALERT, warningButtonText.c_str(), warningIconColor, backgroundColor))
	{
		warningButtonClicked = true;
		filterDirty = true;
	}

	if (!showWarningButtonBorder)
	{
		ImGui::PopStyleVar(1);
	}

	if (warningButtonClicked)
	{
		showWarnings = !showWarnings;
		showWarningButtonBorder = !showWarningButtonBorder;
		warningButtonClicked = false;
	}

	ImGui::SameLine();
	ImGui::Text("");
	ImGui::SameLine();

	if (!showInfoButtonBorder)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
	}

	if (UI::IconButton("  " ICON_MDI_INFORMATION, infoButtonText.c_str(), infoIconColor, backgroundColor))
	{
		infoButtonClicked = true;
		filterDirty = true;
	}

	if (!showInfoButtonBorder)
	{
		ImGui::PopStyleVar(1);
	}

	if (infoButtonClicked)
	{
		showInfo = !showInfo;
		showInfoButtonBorder = !showInfoButtonBorder;
		infoButtonClicked = false;
	}

	ImGui::SameLine();
	ImGui::Text("");
	ImGui::SameLine();
	ImGui::Text("");
	ImGui::SameLine();

	if (!showAutoScrollButtonBorder)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
	}

	if (UI::IconButton("  " ICON_MDI_ARROW_DOWN, " Auto-scroll ", autoScrollIconColor, backgroundColor))
	{
		autoScrollButtonClicked = true;
	}

	if (!showAutoScrollButtonBorder)
	{
		ImGui::PopStyleVar(1);
	}

	if (autoScrollButtonClicked)
	{
		autoScroll = !autoScroll;
		showAutoScrollButtonBorder = !showAutoScrollButtonBorder;
		autoScrollButtonClicked = false;
	}

	ImGui::SameLine();
	ImGui::Text("");
	ImGui::SameLine();

	if (UI::IconButton("  " ICON_MDI_CLOSE, " Clear ", errorIconColor, backgroundColor))
	{
		logLines.clear();
		selectedIds.clear();
		filterDirty = true;
	}

	ImGui::SameLine();
	ImGui::Text("");
	ImGui::SameLine();
	ImGui::Text("");
	ImGui::SameLine();

	std::string hint = std::format("{} Search message...", ICON_MDI_MAGNIFY);

	ImGui::PushItemWidth(-1);

	if (ImGui::InputTextWithHint("##SearchMessage", hint.c_str(), searchBuffer, IM_ARRAYSIZE(searchBuffer)))
	{
		filterDirty = true;
	}

	ImGui::PopItemWidth();
}

void Console::RenderTable()
{
	const bool noFilter = showErrors && showWarnings && showInfo && searchBuffer[0] == '\0';

	if (!noFilter && filterDirty)
	{
		RebuildFilteredIndices();
	}

	if (!ImGui::BeginTable(
		"ConsoleTable",
		3,
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_Reorderable |
		ImGuiTableFlags_Hideable |
		ImGuiTableFlags_Borders |
		ImGuiTableFlags_NoBordersInBody |
		ImGuiTableFlags_SizingFixedFit |
		ImGuiTableFlags_ScrollY))
	{
		return;
	}

	ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
	ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed);
	ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);

	ImGui::TableHeadersRow();

	ImGuiListClipper clipper;

	if (noFilter)
	{
		clipper.Begin(static_cast<int32_t>(logLines.size()));
	}
	else
	{
		clipper.Begin(static_cast<int32_t>(filteredIndices.size()));
	}

	while (clipper.Step())
	{
		for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
		{
			int32_t index = noFilter ? row : filteredIndices[row];
			const LogLine& log = logLines[index];

			const bool isSelected = selectedIds.contains(log.id);
			ImVec4 color = GetTextColor(log.level);

			ImGui::PushID(log.id);
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);

			char label[32];

			sprintf_s(label, "%zu", log.id);

			ImGui::PushStyleColor(ImGuiCol_Text, color);

			if (ImGui::Selectable(label, isSelected, ImGuiSelectableFlags_SpanAllColumns))
			{
				if (ImGui::GetIO().KeyCtrl)
				{
					if (isSelected)
					{
						selectedIds.erase(log.id);
					}
					else
					{
						selectedIds.insert(log.id);
					}
				}
				else
				{
					selectedIds.clear();
					selectedIds.insert(log.id);
				}
			}

			ImGui::PopStyleColor();

			if (ImGui::BeginPopupContextItem())
			{
				std::string copySelectedMessageLabel = std::format("  {} Copy selected message(s) ", ICON_MDI_CONTENT_COPY);
				std::string copySelectedRowLabel = std::format("  {} Copy selected row(s) ", ICON_MDI_CONTENT_COPY);
				std::string deleteMessageLabel = std::format("  {} Delete selected message(s) ", ICON_MDI_CLOSE);

				if (ImGui::MenuItem(copySelectedMessageLabel.c_str()))
				{
					std::string content;

					for (const auto& l : logLines)
					{
						if (selectedIds.contains(l.id))
							content += l.text + "\n";
					}

					CopyToClipboard(content);
				}

				if (ImGui::MenuItem(copySelectedMessageLabel.c_str()))
				{
					std::string content;

					for (const auto& l : logLines)
					{
						if (selectedIds.contains(l.id))
							content += l.ToString() + "\n";
					}

					CopyToClipboard(content);
				}

				if (ImGui::MenuItem(deleteMessageLabel.c_str()))
				{
					std::erase_if(logLines, [&](const LogLine& l)
						{
							return selectedIds.contains(l.id);
						});

					selectedIds.clear();
					filterDirty = true;
				}

				ImGui::EndPopup();
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::TextColored(color, "%s", log.time.c_str());

			ImGui::TableSetColumnIndex(2);
			ImGui::TextColored(color, "%s", log.text.c_str());

			ImGui::PopID();
		}
	}

	if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
	{
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::EndTable();
}

void Console::AddLogLine(spdlog::level::level_enum level, const std::string& text)
{
	logLines.push_back(LogLine{ logLines.size(), level, std::string(text.c_str(), text.size()), GetCurrentTime() });

	filterDirty = true;
}

bool Console::PassFilter(const LogLine& log) const
{
	if (!showErrors && log.level == spdlog::level::err)
	{
		return false;
	}

	if (!showWarnings && log.level == spdlog::level::warn)
	{
		return false;
	}

	if (!showInfo && log.level == spdlog::level::info)
	{
		return false;
	}

	if (searchBuffer[0] != '\0' && !log.text.contains(searchBuffer))
	{
		return false;
	}

	return true;
}

void Console::RebuildFilteredIndices()
{
	filteredIndices.clear();

	for (size_t i = 0; i < logLines.size(); i++)
	{
		if (PassFilter(logLines[i]))
		{
			filteredIndices.push_back(i);
		}
	}

	filterDirty = false;
}

ImColor Console::GetTextColor(const spdlog::level::level_enum level)
{
	switch (level)
	{
		case spdlog::level::level_enum::info:
			return infoIconColor;
		case spdlog::level::level_enum::warn:
			return warningIconColor;
		case spdlog::level::level_enum::err:
			return errorIconColor;
		default:
			return ImGui::GetStyleColorVec4(ImGuiCol_Text);
	}
}

void Console::CopyToClipboard(const std::string& string)
{
	if (!OpenClipboard(nullptr))
	{
		return;
	}

	if (!EmptyClipboard())
	{
		return;
	}

	HGLOBAL globalData = GlobalAlloc(GMEM_MOVEABLE, string.size() + 1);

	if (!globalData)
	{
		CloseClipboard();

		return;
	}

	auto globalDataPtr = GlobalLock(globalData);

	if (!globalDataPtr)
	{
		CloseClipboard();
		GlobalFree(globalData);

		return;
	}

	memset(globalDataPtr, 0, string.size() + 1);
	memcpy(globalDataPtr, string.c_str(), string.size());

	GlobalUnlock(globalData);

	SetClipboardData(CF_TEXT, globalData);
	CloseClipboard();
}

std::string Console::GetCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(now);
	std::tm localTime;
	std::stringstream stringstream;

	localtime_s(&localTime, &time);

	stringstream << std::put_time(&localTime, "%H:%M:%S");

	return stringstream.str();
}
