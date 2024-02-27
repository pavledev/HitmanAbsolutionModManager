#include <imgui.h>

#include <IconsMaterialDesign.h>

#include <UI/ModSelector.h>
#include <ModManager.h>
#include <Utility/UI.h>
#include <Utility/FileDialog.h>

void ModSelector::Draw()
{
    ImGui::PushFont(ModManager::GetInstance().GetImGuiRenderer().GetBoldFont());

    const bool isWindowVisible = ImGui::Begin(ICON_MD_TOKEN " Mods", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    ImGui::PushFont(ModManager::GetInstance().GetImGuiRenderer().GetRegularFont());

    if (isWindowVisible)
    {
        static char mod[256]{ "" };
        std::string hint = std::format("{} Search mod...", ICON_MD_SEARCH);

        ImGui::PushItemWidth(500);
        ImGui::InputTextWithHint("##SearchMod", hint.c_str(), mod, IM_ARRAYSIZE(mod));
        ImGui::PopItemWidth();

        ImGui::SameLine();

        if (UI::IconButton(ICON_MD_ADD, " Add Mod "))
        {
            std::string filePath = FileDialog::OpenFileDialog();

            ModManager::GetInstance().AddMod(filePath);
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Available Mods");
        ImGui::Spacing();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);

        std::map<std::string, ModManager::Mod>& availableMods = ModManager::GetInstance().GetAvailableMods();
        unsigned int index = 0;

        for (auto it = availableMods.begin(); it != availableMods.end(); ++it)
        {
            std::string checkboxLabel = std::format("##Mod{}", index);

            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, ImGui::GetTextLineHeightWithSpacing() * 2.f);

            ImGui::Checkbox(checkboxLabel.c_str(), &it->second.isEnabled);
            ImGui::NextColumn();

            ImGui::Text(it->second.name.c_str());
            ImGui::Text(it->second.description.c_str());

            ImGui::Columns(1);

            index++;
        }

        ImGui::EndChild();
        ImGui::Separator();

        ImGui::BeginChild("ButtonRegion", ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 2), false);

        if (ImGui::Button("Apply"))
        {
            EnableSelectedMods();
        }

        ImGui::SameLine();

        if (ImGui::Button("Refresh List"))
        {
            ModManager::GetInstance().FindAvailableMods();
        }

        ImGui::EndChild();
    }

    ImGui::PopFont();
    ImGui::End();
    ImGui::PopFont();
}

void ModSelector::EnableSelectedMods()
{
    std::map<std::string, ModManager::Mod>& availableMods = ModManager::GetInstance().GetAvailableMods();

    for (auto it = availableMods.begin(); it != availableMods.end(); ++it)
    {
        if (it->second.isEnabled)
        {
            ModManager::GetInstance().GenerateResources(it->second.contentFolderPath);
        }
    }
}
