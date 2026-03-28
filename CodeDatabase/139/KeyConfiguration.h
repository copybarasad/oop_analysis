#pragma once
#include "DataTypes/InputEvent.h"
#include "JsonLib/json.hpp"
#include <unordered_map>
#include <fstream>


class KeyConfiguration
{
private:
    std::string filename;

    class ConfigurationWrapper {
    private:
        std::ifstream file;
    public:
        ConfigurationWrapper(const std::string& filename);

        bool loadKeys(std::unordered_map<KeyCode, KeyCode>& keyMap);

        KeyCode parseAction(const std::string& action) const;

        KeyCode parseKeyCode(const std::string& key) const;

        ~ConfigurationWrapper();
    };

    std::unordered_map<KeyCode, KeyCode> keyMap;
public:
    KeyConfiguration(const std::string& filename);
    KeyCode getKey(KeyCode key) const;
private:
    void loadDefaultKeyMap();
};