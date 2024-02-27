#pragma once

#include <map>
#include <filesystem>
#include <unordered_map>

#include <Rendering/DirectXRenderer.h>
#include <Rendering/ImGuiRenderer.h>
#include <UI/ModSelector.h>
#include <UI/Console.h>

class ModManager
{
public:
	struct Mod
	{
		std::string name;
		std::string description;
		std::string contentFolderPath;
		bool isEnabled;
	};

	struct Resource
	{
		unsigned long long runtimeResourceID;
		void* resourceData;
		unsigned int resourceDataSize;
	};

	~ModManager();
	static ModManager& GetInstance();
	bool Setup();
	void Start();
	void Render();
	void RenderContent();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void FindAvailableMods();
	void LoadAllMods();
	void UnloadAllMods();
	void GetModInfo(Mod& mod, const std::filesystem::path& manifestJsonFileEntry);
	std::map<std::string, Mod>& GetAvailableMods();
	void AddMod(const std::string& modFilePath);

	const ImGuiRenderer& GetImGuiRenderer() const;
	const ModSelector& GetModSelector() const;

	void GenerateResources(const std::filesystem::path& contentFolderPath);
	void GenerateResource(std::vector<Resource>& resources, const std::filesystem::path& fileEntry);
	void GenerateTEMPAndTBLU(std::vector<Resource>& resources, const std::filesystem::path& jsonFileEntry);
	void GeneratePatchFile(const std::vector<Resource>& resources, const std::string& headerLibraryFileName);

	static void GetResourceReferences(const unsigned long long resourceRuntimeResourceID, const std::string& headerLibraryFilePath, std::vector<unsigned long long>& references);
	static const char* GetHeaderLibraryResourceID(const std::string& headerLibraryFileName);
	static std::string ConvertResourceIDToFilePath(const std::string& resourceID);

private:
	ModManager();
	ModManager(const ModManager& other) = delete;
	ModManager& operator=(const ModManager& other) = delete;

	WNDCLASSEXW wc;
	HWND hwnd;
	inline static unsigned int resizeWidth;
	inline static unsigned int resizeHeight;

	DirectXRenderer directXRenderer;
	ImGuiRenderer imGuiRenderer;

	std::map<std::string, Mod> availableMods;

	ModSelector modSelector;
	Console console;
};
