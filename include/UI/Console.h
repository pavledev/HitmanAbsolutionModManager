#pragma once

#include <vector>
#include <unordered_set>

#include <imgui.h>

#include "spdlog/spdlog.h"

#undef GetCurrentTime

class Console
{
public:
	Console();
	void Render();

	void AddLogLine(spdlog::level::level_enum level, const std::string& text);

private:
	struct LogLine
	{
		std::string ToString() const
		{
			std::string level;

			switch (this->level)
			{
				case spdlog::level::level_enum::info:
					level = "Info";
				case spdlog::level::level_enum::warn:
					level = "Warning";
				case spdlog::level::level_enum::err:
					level = "Error";
			}

			return std::format("{} {} {}", time, level, text);
		}

		size_t id;
		spdlog::level::level_enum level;
		std::string text;
		std::string time;
	};

	void RenderButtons();
	void RenderTable();

	bool PassFilter(const LogLine& log) const;
	void RebuildFilteredIndices();

	ImColor GetTextColor(const spdlog::level::level_enum level);

	static void CopyToClipboard(const std::string& string);

	static std::string GetCurrentTime();

	std::vector<LogLine> logLines;
	std::unordered_set<int> selectedIds;

	char searchBuffer[256]{ "" };

	ImColor errorIconColor;
	ImColor warningIconColor;
	ImColor infoIconColor;
	ImVec4 backgroundColor;
	ImVec4 autoScrollIconColor;

	bool showErrors;
	bool showWarnings;
	bool showInfo;
	bool autoScroll;

	bool showErrorButtonBorder;
	bool showWarningButtonBorder;
	bool showInfoButtonBorder;
	bool showAutoScrollButtonBorder;

	bool errorButtonClicked;
	bool warningButtonClicked;
	bool infoButtonClicked;
	bool autoScrollButtonClicked;

	std::vector<int> filteredIndices;
	bool filterDirty = true;
};
