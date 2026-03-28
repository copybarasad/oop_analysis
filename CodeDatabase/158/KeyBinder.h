#pragma once

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <functional>

class KeyBinder 
{
private:
    std::string keyPreset = "WASDFRPUOKLM";
    std::string currentPresetName = "default";
    std::map<std::string, std::string> presets;
    
    static const std::string HASH_MARKER;
    
    std::string calculateHash(const std::string& data) const;
    
public:
    enum Action 
    {
        MOVE_UP = 0,
        MOVE_DOWN = 1,
        MOVE_LEFT = 2,
        MOVE_RIGHT = 3,
        ATTACK = 4,
        SWITCH_ATTACK = 5,
        SHOP = 6,
        UPGRADE = 7,
        SAVE_MENU = 8,
        LOAD_MENU = 9,
        TOGGLE_ANIMATIONS = 10,
        KEY_CONFIG = 11,
        BACK_TO_MENU = 12,
        ACTION_COUNT = 13
    };
    
    KeyBinder();
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    void setKey(int action, char key);
    char getKey(int action) const;
    void resetToDefault();
    void setPreset(const std::string& presetName, const std::string& keys);
    bool loadPreset(const std::string& presetName);
    std::string getCurrentPresetName() const;
    void displayCurrentBindings() const;
};