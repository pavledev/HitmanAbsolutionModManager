#pragma once

#include <string>
#include <unordered_map>

class ResourceIDRegistry
{
public:
	static ResourceIDRegistry& GetInstance();
	void Load();
	unsigned long long GetRuntimeResourceID(const std::string& resourceID) const;
	const bool IsLoaded() const;

private:
	ResourceIDRegistry() = default;
	ResourceIDRegistry(const ResourceIDRegistry& other) = delete;
	ResourceIDRegistry& operator=(const ResourceIDRegistry& other) = delete;

	std::unordered_map<std::string, unsigned long long> resourceIDsToRuntimeResourceIDs;
	bool isLoaded = false;
};
