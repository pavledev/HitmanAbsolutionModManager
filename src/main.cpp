#include <filesystem>

#include <ModManager.h>

int main()
{
    std::filesystem::path cwd = std::filesystem::current_path();
    char filePath[MAX_PATH];

    GetModuleFileNameA(NULL, filePath, sizeof(filePath));

    if (std::filesystem::path(filePath).parent_path().string().ends_with("ModManager"))
    {
        std::filesystem::current_path(std::filesystem::path(filePath).parent_path().parent_path());
    }

    ModManager& modManager = ModManager::GetInstance();

    if (!modManager.Setup())
    {
        return 0;
    }

    modManager.Start();

    return 0;
}
