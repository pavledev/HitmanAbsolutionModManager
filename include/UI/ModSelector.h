#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <map>

class ModSelector
{
private:
    struct Mod
    {
        std::string name;
        std::string description;
        bool enabled;
    };

public:
    void Draw();
    void EnableSelectedMods();
};
