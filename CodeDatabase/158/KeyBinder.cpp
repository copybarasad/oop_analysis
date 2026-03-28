#include "KeyBinder.h"

const std::string KeyBinder::HASH_MARKER = "KEYBINDER_HASH";

KeyBinder::KeyBinder() 
{
    presets["default"] = "WASDFRPUOKLM";
    presets["arrows"] = "UDLRFRPUOKLM";
    presets["numpad"] = "8426*+RPUOKLM";
    resetToDefault();
}

std::string KeyBinder::calculateHash(const std::string& data) const 
{
    std::hash<std::string> hasher;
    size_t hashValue = hasher(data);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashValue;
    return ss.str();
}

bool KeyBinder::loadFromFile(const std::string& filename) 
{
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    
    std::string firstLine;
    std::getline(file, firstLine);
    
    if (firstLine.find(HASH_MARKER) != 0) 
    {
        file.close();
        return false;
    }
    
    std::string storedHash = firstLine.substr(firstLine.find('=') + 1);
    
    std::string fileContent;
    std::getline(file, fileContent);
    file.close();
    
    std::string calculatedHash = calculateHash(fileContent);
    
    if (storedHash != calculatedHash) 
    {
        std::cerr << "Key config file corrupted! Hash mismatch.\n";
        return false;
    }
    
    if (fileContent.length() >= ACTION_COUNT) 
    {
        keyPreset = fileContent;
        currentPresetName = "custom";
        return true;
    }
    
    return false;
}

bool KeyBinder::saveToFile(const std::string& filename) 
{
    std::string hash = calculateHash(keyPreset);
    
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;
    
    outFile << HASH_MARKER << "=" << hash << "\n";
    outFile << keyPreset;
    outFile.close();
    
    currentPresetName = filename;
    return true;
}

void KeyBinder::setKey(int action, char key) 
{
    if (action >= 0 && action < ACTION_COUNT) 
    {
        keyPreset[action] = toupper(key);
    }
}

char KeyBinder::getKey(int action) const 
{
    if (action >= 0 && action < ACTION_COUNT) 
    {
        return keyPreset[action];
    }
    return '\0';
}

void KeyBinder::resetToDefault() 
{
    keyPreset = presets["default"];
    currentPresetName = "default";
}

void KeyBinder::setPreset(const std::string& presetName, const std::string& keys) 
{
    if (keys.length() >= ACTION_COUNT) 
    {
        presets[presetName] = keys;
    }
}

bool KeyBinder::loadPreset(const std::string& presetName) 
{
    auto it = presets.find(presetName);
    if (it != presets.end() && it->second.length() >= ACTION_COUNT) 
    {
        keyPreset = it->second;
        currentPresetName = presetName;
        return true;
    }
    return false;
}

std::string KeyBinder::getCurrentPresetName() const 
{
    return currentPresetName;
}

void KeyBinder::displayCurrentBindings() const 
{
    const char* actionNames[] = 
    {
        "Move Up", "Move Down", "Move Left", "Move Right",
        "Attack", "Switch Attack", "Open Shop", "Upgrade Menu",
        "Save Menu", "Load Menu", "Toggle Animations", "Key Config",
        "Back to Menu"
    };
    
    std::cout << "\n=== Current Key Bindings (" << currentPresetName << ") ===\n";
    for (int i = 0; i < ACTION_COUNT; i++) 
    {
        std::cout << actionNames[i] << ": '" << keyPreset[i] << "'\n";
    }
    std::cout << "==============================\n";
}