#include <filesystem>
#include <fstream>
#include <thread>

#include <imgui_internal.h>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>

#define MINI_CASE_SENSITIVE

#include <ini.h>

#include <libzippp/libzippp.h>

#include <Glacier/Entity/STemplateEntity.h>
#include <Glacier/Entity/STemplateEntityBlueprint.h>
#include <Glacier/Resource/SResourceHeaderHeader.h>
#include <Glacier/Resource/ZResourceHeaderReader.h>

#include <ModManager.h>
#include <Timer.h>
#include <Registry/ResourceIDRegistry.h>
#include <Registry/EnumRegistry.h>
#include <Registry/TypeRegistry.h>
#include <Utility/StringUtility.h>
#include <Hash.h>
#include <IO/BinaryReader.h>
#include <Global.h>

ModManager::ModManager()
{
    hwnd = nullptr;

    ResourceIDRegistry& resourceIDRegistry = ResourceIDRegistry::GetInstance();
    EnumRegistry& enumRegistry = EnumRegistry::GetInstance();
    TypeRegistry& typeRegistry = TypeRegistry::GetInstance();
    std::thread thread = std::thread(&ResourceIDRegistry::Load, &resourceIDRegistry);
    std::thread thread2 = std::thread(&EnumRegistry::Load, &enumRegistry);
    std::thread thread3 = std::thread(&TypeRegistry::Load, &typeRegistry);

    thread.detach();
    thread2.detach();
    thread3.detach();

    if (!std::filesystem::is_directory("ModManager\\mods"))
    {
        std::filesystem::create_directory("ModManager\\mods");
    }

    if (!std::filesystem::is_directory("ModManager\\patches"))
    {
        std::filesystem::create_directory("ModManager\\patches");
    }
}

ModManager::~ModManager()
{
    imGuiRenderer.Cleanup();
    directXRenderer.CleanupD3DDevice();
    DestroyWindow(hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

ModManager& ModManager::GetInstance()
{
    static ModManager instance;

    return instance;
}

bool ModManager::Setup()
{
    wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Hitman Absolution Mod Manager", nullptr };

    RegisterClassExW(&wc);

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    hwnd = CreateWindowExW(0, wc.lpszClassName, L"Hitman Absolution Mod Manager", WS_OVERLAPPEDWINDOW, 0, 0, width, height, nullptr, nullptr, wc.hInstance, nullptr);

    Logger& logger = Logger::GetInstance();

    if (!directXRenderer.Setup(hwnd, &wc))
    {
        logger.Log(Logger::Level::Error, "Failed to setup DirectX renderer!");

        return false;
    }

    ShowWindow(hwnd, SW_MAXIMIZE);
    UpdateWindow(hwnd);

    if (!imGuiRenderer.Setup(directXRenderer.GetD3D11Device(), directXRenderer.GetD3D11DeviceContext(), hwnd))
    {
        logger.Log(Logger::Level::Error, "Failed to setup ImGui renderer!");

        return false;
    }

    LoadAllMods();

    return true;
}

void ModManager::Start()
{
    Timer::SetFPSLimit();

    bool quit = false;

    while (true)
    {
        Timer::Tick();

        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;

        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                quit = true;
            }
        }

        if (quit)
        {
            break;
        }

        if (resizeWidth != 0 && resizeHeight != 0)
        {
            directXRenderer.GetSwapChain()->Resize(directXRenderer.GetD3D11Device(), resizeWidth, resizeHeight);
            imGuiRenderer.SetScale();

            resizeWidth = 0;
            resizeHeight = 0;
        }

        Render();
    }
}

void ModManager::Render()
{
    ImGuiIO& io = ImGui::GetIO();

    // Start the Dear ImGui frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    RenderContent();

    // Rendering
    ImGui::Render();

    directXRenderer.ClearBackBuffer();
    directXRenderer.SetBackBufferAsRenderTarget();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    directXRenderer.GetSwapChain()->Present();
}

void ModManager::RenderContent()
{
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;

    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("###DocumentRoot", nullptr, windowFlags);
    ImGui::PopStyleVar(3);

    ImGuiID rootDockspaceID = ImGui::GetID("RootDockspace");
    ImGui::DockSpace(rootDockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoSplit);

    modSelector.Draw();
    console.Draw();

    ImGui::End();
}

LRESULT ModManager::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
        case WM_SIZE:
        {
            if (wParam == SIZE_MINIMIZED)
            {
                return 0;
            }

            resizeWidth = (UINT)LOWORD(lParam);
            resizeHeight = (UINT)HIWORD(lParam);

            return 0;
        }
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_DPICHANGED:
        {
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            {
                const RECT* suggested_rect = reinterpret_cast<RECT*>(lParam);

                SetWindowPos(hwnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }

            break;
        }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void ModManager::FindAvailableMods()
{
    std::filesystem::path modsFolderPath = std::format("{}\\ModManager\\mods", std::filesystem::current_path().string());

    if (std::filesystem::exists(modsFolderPath))
    {
        availableMods.clear();

        for (const auto& entry : std::filesystem::recursive_directory_iterator(modsFolderPath))
        {
            if (entry.is_directory() && entry.path().filename() == "content")
            {
                continue;
            }

            if (entry.path().filename().string() == "manifest.json")
            {
                Mod mod;

                GetModInfo(mod, entry.path());

                availableMods.insert(std::make_pair(mod.name, mod));
            }
        }
    }
    else
    {
        Logger::GetInstance().Log(Logger::Level::Warning, "mods directory not found!");
    }
}

void ModManager::LoadAllMods()
{
    FindAvailableMods();

    std::filesystem::path iniFilePath = std::format("{}\\ModManager\\mods.ini", std::filesystem::current_path().string());

    if (std::filesystem::exists(iniFilePath))
    {
        mINI::INIFile iniFile = mINI::INIFile(iniFilePath.string());
        mINI::INIStructure iniStructure;

        iniFile.read(iniStructure);

        for (auto& mod : iniStructure)
        {
            auto availableModsIterator = availableMods.find(mod.first);

            if (availableModsIterator!= availableMods.end())
            {
                availableModsIterator->second.isEnabled = true;
            }
        }
    }
}

void ModManager::UnloadAllMods()
{

}

void ModManager::GetModInfo(Mod& mod, const std::filesystem::path& manifestJsonFileEntry)
{
    std::ifstream inputFile = std::ifstream(manifestJsonFileEntry);
    rapidjson::IStreamWrapper streamWrapper = rapidjson::IStreamWrapper(inputFile);
    rapidjson::Document document;

    document.ParseStream(streamWrapper);

    mod.name = document["name"].GetString();
    mod.description = document["description"].GetString();
    mod.contentFolderPath = std::format("{}\\content", manifestJsonFileEntry.parent_path().string());
}

std::map<std::string, ModManager::Mod>& ModManager::GetAvailableMods()
{
    return availableMods;
}

void ModManager::AddMod(const std::string& modFilePath)
{
    libzippp::ZipArchive zipArchive = libzippp::ZipArchive(modFilePath);

    zipArchive.open(libzippp::ZipArchive::ReadOnly);

    const std::vector<libzippp::ZipEntry> entries = zipArchive.getEntries();

    for (auto it = entries.begin(); it != entries.end(); ++it)
    {
        const libzippp::ZipEntry entry = *it;
        const std::string name = entry.getName();

        if (entry.isDirectory())
        {
            std::filesystem::create_directories("ModManager/mods/" + name);
        }
        else
        {
            const void* data = entry.readAsBinary();
            const unsigned long long size = entry.getSize();
            BinaryWriter binaryWriter = BinaryWriter("ModManager/mods/" + name);

            binaryWriter.Write(data, size);
        }
    }

    zipArchive.close();
}

const ImGuiRenderer& ModManager::GetImGuiRenderer() const
{
    return imGuiRenderer;
}

const ModSelector& ModManager::GetModSelector() const
{
    return modSelector;
}

void ModManager::GenerateResources(const std::filesystem::path& contentFolderPath)
{
    std::unordered_map<std::filesystem::path, std::vector<std::filesystem::path>> folders;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(contentFolderPath))
    {
        if (!entry.is_directory())
        {
            folders[entry.path().parent_path().filename()].push_back(entry.path());
        }
    }

    std::vector<Resource> resources;

    for (auto it = folders.begin(); it != folders.end(); ++it)
    {
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            GenerateResource(resources, it->second[i]);
        }

        GeneratePatchFile(resources, it->first.string());

        resources.clear();
    }
}

void ModManager::GenerateResource(std::vector<Resource>& resources, const std::filesystem::path& fileEntry)
{
    const std::string& fileName = fileEntry.filename().string();

    if (fileName.ends_with("entity.json"))
    {
        GenerateTEMPAndTBLU(resources, fileEntry);
    }
}

void ModManager::GenerateTEMPAndTBLU(std::vector<Resource>& resources, const std::filesystem::path& jsonFileEntry)
{
    std::ifstream inputFile = std::ifstream(jsonFileEntry);
    rapidjson::IStreamWrapper streamWrapper = rapidjson::IStreamWrapper(inputFile);
    rapidjson::Document document;

    document.ParseStream(streamWrapper);

    Resource tempResource;
    Resource tbluResource;

    tempResource.runtimeResourceID = ZRuntimeResourceID::QueryRuntimeResourceID(document["templateEntityResourceID"].GetString());
    tbluResource.runtimeResourceID = ZRuntimeResourceID::QueryRuntimeResourceID(document["templateEntityBlueprintResourceID"].GetString());

    STemplateEntity* templateEntity = STemplateEntity::DeserializeFromJson(document);
    STemplateEntityBlueprint* templateEntityBlueprint = STemplateEntityBlueprint::DeserializeFromJson(document);

    const std::string headerLibraryFileName = jsonFileEntry.parent_path().filename().string();
    const char* headerLibraryResourceID = GetHeaderLibraryResourceID(headerLibraryFileName);
    const std::string headerLibraryFilePath = ConvertResourceIDToFilePath(headerLibraryResourceID);
    ZBinarySerializer binarySerializer;
    ZBinarySerializer binarySerializer2;
    std::vector<unsigned long long> references;

    GetResourceReferences(tempResource.runtimeResourceID, headerLibraryFilePath, references);

    std::unordered_map<unsigned long long, unsigned int> references2;

    for (unsigned int i = 0; i < references.size(); ++i)
    {
        references2.insert(std::make_pair(references[i], i));
    }

    binarySerializer.SerializeToMemory(templateEntity, &tempResource.resourceData, tempResource.resourceDataSize, &references2);
    binarySerializer2.SerializeToMemory(templateEntityBlueprint, &tbluResource.resourceData, tbluResource.resourceDataSize);

    resources.push_back(tempResource);
    resources.push_back(tbluResource);

    delete templateEntity;
    delete templateEntityBlueprint;

    propertyDataPointersToReferenceCounts.clear();
}

void ModManager::GeneratePatchFile(const std::vector<Resource>& resources, const std::string& headerLibraryFileName)
{
    const std::string patchesFolderPath = std::format("ModManager\\patches\\{}", headerLibraryFileName);

    if (!std::filesystem::is_directory(patchesFolderPath))
    {
        std::filesystem::create_directory(patchesFolderPath);
    }

    unsigned int patchNumber = -1;
    std::set<unsigned int> patchVersions;

    for (const auto& entry : std::filesystem::directory_iterator(patchesFolderPath))
    {
        const std::string fileName = entry.path().filename().string();
        const unsigned int patchNumber2 = std::strtoul(fileName.substr(5, fileName.find(".") - 5).c_str(), nullptr, 10);

        patchVersions.insert(patchNumber2);
    }

    if (!patchVersions.empty())
    {
        patchNumber = *patchVersions.rbegin();
    }

    const std::string patchFilePath = std::format("{}\\patch{}.rpkg", patchesFolderPath, patchNumber + 1);
    BinaryWriter binaryWriter = BinaryWriter(patchFilePath);

    binaryWriter.Write(resources.size());

    for (size_t i = 0; i < resources.size(); ++i)
    {
        binaryWriter.Write(resources[i].runtimeResourceID);
        binaryWriter.Write(0);
        binaryWriter.Write(resources[i].resourceDataSize);
    }

    for (size_t i = 0; i < resources.size(); ++i)
    {
        const size_t resourceDataOffset = binaryWriter.GetPosition();

        binaryWriter.Seek(sizeof(unsigned int) + i * sizeof(Resource) + sizeof(unsigned long long));
        binaryWriter.Write(resourceDataOffset);
        binaryWriter.Seek(resourceDataOffset);

        binaryWriter.Write(resources[i].resourceData, resources[i].resourceDataSize);
    }
}

void ModManager::GetResourceReferences(const unsigned long long resourceRuntimeResourceID, const std::string& headerLibraryFilePath, std::vector<unsigned long long>& references)
{
    BinaryReader headerLibraryBinaryReader = BinaryReader(headerLibraryFilePath);

    headerLibraryBinaryReader.Seek(4, SeekOrigin::Begin);

    const unsigned int referencesChunkSize = headerLibraryBinaryReader.Read<unsigned int>();

    headerLibraryBinaryReader.Seek(referencesChunkSize + 24 + 16, SeekOrigin::Begin);

    unsigned int chunksStartOffset = headerLibraryBinaryReader.GetPosition();
    chunksStartOffset += headerLibraryBinaryReader.Read<unsigned int>();

    headerLibraryBinaryReader.Seek(chunksStartOffset - 4);

    const unsigned int chunkCount = headerLibraryBinaryReader.Read<unsigned int>();

    for (unsigned int i = 0; i < chunkCount; ++i)
    {
        const unsigned int chunkOffset = chunksStartOffset + 0x50 * i; //0x50 is size of SHeaderLibraryChunk

        headerLibraryBinaryReader.Seek(chunkOffset + 0x44, SeekOrigin::Begin);

        unsigned int runtimeResourceIDsStartOffset = headerLibraryBinaryReader.GetPosition();
        runtimeResourceIDsStartOffset += headerLibraryBinaryReader.Read<unsigned int>();

        headerLibraryBinaryReader.Seek(runtimeResourceIDsStartOffset - 4);

        const unsigned int ridMappingIDsCount = headerLibraryBinaryReader.Read<unsigned int>();
        unsigned int indexInLibrary = -1;

        for (unsigned int j = 0; j < ridMappingIDsCount; ++j)
        {
            headerLibraryBinaryReader.Seek(runtimeResourceIDsStartOffset + j * 0x8, SeekOrigin::Begin);

            const unsigned long long runtimeResourceID = headerLibraryBinaryReader.Read<unsigned long long>();

            if (runtimeResourceID == resourceRuntimeResourceID)
            {
                indexInLibrary = j;

                break;
            }
        }

        if (indexInLibrary != -1)
        {
            headerLibraryBinaryReader.Seek(chunkOffset + 0x2C, SeekOrigin::Begin);

            unsigned int resourceHeadersStartOffset = headerLibraryBinaryReader.GetPosition();
            resourceHeadersStartOffset += headerLibraryBinaryReader.Read<unsigned int>();

            headerLibraryBinaryReader.Seek(resourceHeadersStartOffset + indexInLibrary * 0xC, SeekOrigin::Begin); //0xC is size of TArray

            unsigned int resourceHeaderOffset = headerLibraryBinaryReader.GetPosition();
            resourceHeaderOffset += headerLibraryBinaryReader.Read<unsigned int>();

            headerLibraryBinaryReader.Seek(resourceHeaderOffset, SeekOrigin::Begin);

            const SResourceHeaderHeader resourceHeaderHeader = headerLibraryBinaryReader.Read<SResourceHeaderHeader>();
            unsigned char* referencesChunk = headerLibraryBinaryReader.Read<unsigned char>(resourceHeaderHeader.m_nReferencesChunkSize);
            const ZResourceHeaderReader resourceHeaderReader = ZResourceHeaderReader(&resourceHeaderHeader, referencesChunk);
            const unsigned int referenceCount = resourceHeaderReader.GetNumResourceIdentifiers();

            for (unsigned int j = 0; j < referenceCount; ++j)
            {
                const ZRuntimeResourceID runtimeResourceID = resourceHeaderReader.GetResourceIdentifier(j);

                references.push_back(runtimeResourceID);
            }

            delete[] referencesChunk;

            break;
        }
    }
}

const char* ModManager::GetHeaderLibraryResourceID(const std::string& headerLibraryFileName)
{
    static std::unordered_map<std::string, std::string> headerLibraryFileNamesToResourceIDs;

    if (headerLibraryFileNamesToResourceIDs.empty())
    {
        std::ifstream ifstream = std::ifstream("ModManager/assets/HeaderLibraries.txt");

        if (!ifstream.is_open())
        {
            Logger::GetInstance().Log(Logger::Level::Error, "Failed to open HeaderLibraries.txt!");

            return nullptr;
        }

        std::string line;

        while (getline(ifstream, line))
        {
            const size_t index = line.find_last_of("/");
            const std::string headerLibraryFileName2 = line.substr(index + 1, line.find(".", index) - index - 1);

            headerLibraryFileNamesToResourceIDs.insert(std::make_pair(headerLibraryFileName2, line));
        }

        ifstream.close();
    }

    auto iterator = headerLibraryFileNamesToResourceIDs.find(headerLibraryFileName);

    if (iterator != headerLibraryFileNamesToResourceIDs.end())
    {
        return iterator->second.c_str();
    }

    return nullptr;
}

std::string ModManager::ConvertResourceIDToFilePath(const std::string& resourceID)
{
    std::string resourceID2 = StringUtility::ToLowerCase(resourceID);
    const std::string extension = resourceID2.substr(resourceID2.find_last_of('.') + 1);
    const int index = static_cast<int>(resourceID2.find(':'));
    const int index2 = static_cast<int>(resourceID2.find('?'));
    const int index3 = static_cast<int>(resourceID2.find(']'));
    std::string folderPath = std::format("{}/runtime", std::filesystem::current_path().string());

    if (index2 != -1 && index < index2)
    {
        folderPath += resourceID2.substr(index + 1, index2 - index - 1);
    }
    else
    {
        folderPath += resourceID2.substr(index + 1, index3 - index - 1);
    }

    resourceID2 = resourceID2.substr(0, resourceID2.find_last_of('.') + 1);

    const Hash::MD5Hash md5Hash = Hash::MD5(resourceID2);
    const std::string fileName = Hash::ConvertMD5ToString(md5Hash);

    return std::format("{}/{}.{}", folderPath, fileName, extension);
}
