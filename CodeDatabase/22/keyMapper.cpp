#include "console/keyMapper.h"
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>


std::unordered_map<char, CommandType> 
KeyMapper::getDefaultMap()
{
    return {
        {'W', CommandType::MoveUp},
        {'S', CommandType::MoveDown},
        {'A', CommandType::MoveLeft},
        {'D', CommandType::MoveRight},
        {'F', CommandType::Attack},
        {'E', CommandType::ChangeWeapon},
        {'R', CommandType::OpenHand},
        {'Q', CommandType::Quit},
        {'M', CommandType::Save},
        {'L', CommandType::Load}};
}

bool KeyMapper::validateConfig(std::unordered_map<char, CommandType> &map)
{
    std::unordered_map<CommandType, int> commandCount;
    std::unordered_set<CommandType> buff;

    for (auto &[key, cmd] : map)
    {
        commandCount[cmd]++;
        if (commandCount[cmd] > 1)
        {
            std::cerr << "Warning: Command " << static_cast<int>(cmd) << " mapped to multiple keys.\n";
        }

        buff.insert(cmd);
    }

    if ((size_t)buff.size() != static_cast<size_t>(CommandType::UseSpell)) return false;

    return true; 
}

std::unordered_map<char, CommandType> KeyMapper::loadConfig(const std::string &filename)
{
    std::ifstream file(filename);
    
    if (!file.is_open())
    {
        std::cerr << "Could not open config file. Using default keymap.\n";
        return getDefaultMap();
    }

    nlohmann::json j;
    try
    {
        file >> j;
    }
    catch (...)
    {
        std::cerr << "Invalid JSON in config. Using default keymap.\n";
        return getDefaultMap();
    }

    auto defaultMap = getDefaultMap();
    std::unordered_map<char, CommandType> result;

    if (j.contains("keymap") && j["keymap"].is_object())
    {
        for (auto &[keyStr, cmdStr] : j["keymap"].items())
        {
            if (keyStr.size() != 1)
                continue;

            char key = keyStr[0];
            std::string cmdName = cmdStr.get<std::string>();

            static const std::unordered_map<std::string, CommandType> nameToCmd = {
                {"MoveUp", CommandType::MoveUp},
                {"MoveDown", CommandType::MoveDown},
                {"MoveLeft", CommandType::MoveLeft},
                {"MoveRight", CommandType::MoveRight},
                {"Attack", CommandType::Attack},
                {"ChangeWeapon", CommandType::ChangeWeapon},
                {"OpenHand", CommandType::OpenHand},
                {"Quit", CommandType::Quit},
                {"Save", CommandType::Save},
                {"Load", CommandType::Load}};

            if (nameToCmd.count(cmdName))
            {
                result[key] = nameToCmd.at(cmdName);
            }
        }
    }

    if (!validateConfig(result))
    {
        std::cerr << "Config validation failed. Using default keymap.\n";
        return getDefaultMap();
    }

    return result.empty() ? getDefaultMap() : result;
}