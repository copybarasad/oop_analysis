#include "KeyboardLayout.hpp"
#include <cctype>

KeyboardLayout::KeyboardLayout() {
    setDefaults();
    valid = true;
}

KeyboardLayout::KeyboardLayout(const std::string& configFile) {
    if (!loadFromFile(configFile)) {
        setDefaults();
    }
    valid = true;
}

void KeyboardLayout::setDefaults() {
    KEYUP = StandardKeyboardLayout::KEYUP;
    KEYLEFT = StandardKeyboardLayout::KEYLEFT;
    KEYRIGHT = StandardKeyboardLayout::KEYRIGHT;
    KEYDOWN = StandardKeyboardLayout::KEYDOWN;
    KEYE = StandardKeyboardLayout::KEYE;
    KEYQ = StandardKeyboardLayout::KEYQ;
    KEYESC = StandardKeyboardLayout::KEYESC;
    KEYENTER = StandardKeyboardLayout::KEYENTER;
    KEY1 = StandardKeyboardLayout::KEY1;
    KEY2 = StandardKeyboardLayout::KEY2;
    KEY3 = StandardKeyboardLayout::KEY3;
    KEY4 = StandardKeyboardLayout::KEY4;

    commandToKeys.clear();
    keyToCommand.clear();

    commandToKeys["MOVE_UP"] = {KEYUP};
    commandToKeys["MOVE_LEFT"] = {KEYLEFT};
    commandToKeys["MOVE_RIGHT"] = {KEYRIGHT};
    commandToKeys["MOVE_DOWN"] = {KEYDOWN};
    commandToKeys["USE_ITEM"] = {KEYE};
    commandToKeys["SWAP_WEAPON"] = {KEYQ};
    commandToKeys["MENU"] = {KEYESC};
    commandToKeys["SELECT"] = {KEYENTER};
    commandToKeys["SPELL_1"] = {KEY1};
    commandToKeys["SPELL_2"] = {KEY2};
    commandToKeys["SPELL_3"] = {KEY3};
    commandToKeys["SPELL_4"] = {KEY4};

    keyToCommand[KEYUP] = "MOVE_UP";
    keyToCommand[KEYLEFT] = "MOVE_LEFT";
    keyToCommand[KEYRIGHT] = "MOVE_RIGHT";
    keyToCommand[KEYDOWN] = "MOVE_DOWN";
    keyToCommand[KEYE] = "USE_ITEM";
    keyToCommand[KEYQ] = "SWAP_WEAPON";
    keyToCommand[KEYESC] = "MENU";
    keyToCommand[KEYENTER] = "SELECT";
    keyToCommand[KEY1] = "SPELL_1";
    keyToCommand[KEY2] = "SPELL_2";
    keyToCommand[KEY3] = "SPELL_3";
    keyToCommand[KEY4] = "SPELL_4";
}

char KeyboardLayout::parseKey(const std::string& keyStr) {
    if (keyStr.empty()) return 0;
    if (keyStr == "ESC") return 27;
    if (keyStr == "ENTER") return '\n';
    if (keyStr.size() == 1) {
        return keyStr[0];
    }
    if (keyStr.size() == 3 && keyStr[0] == '\'' && keyStr[2] == '\'') {
        return keyStr[1];
    }
    return 0;
}

bool KeyboardLayout::validateLayout() {
    std::unordered_map<char, std::string> tempKeyToCommand;
    std::unordered_set<std::string> requiredCommands = {
        "MOVE_UP", "MOVE_LEFT", "MOVE_RIGHT", "MOVE_DOWN",
        "USE_ITEM", "SWAP_WEAPON", "MENU", "SELECT",
        "SPELL_1", "SPELL_2", "SPELL_3", "SPELL_4"
    };
    for (const auto& cmd : requiredCommands) {
        if (commandToKeys.find(cmd) == commandToKeys.end() ||
            commandToKeys[cmd].empty()) {
            std::cerr << "Error: Missing binding for command: " << cmd << std::endl;
            return false;
        }
    }
    for (const auto& pair : commandToKeys) {
        const std::string& command = pair.first;
        const std::vector<char>& keys = pair.second;

        for (char key : keys) {
            if (key == 0) continue;
            if (tempKeyToCommand.find(key) != tempKeyToCommand.end()) {
                std::cerr << "Error: Key '" << key << "' assigned to multiple commands: "
                          << tempKeyToCommand[key] << " and " << command << std::endl;
                return false;
            }
            tempKeyToCommand[key] = command;
        }
    }
    for (const auto& pair : commandToKeys) {
        const std::vector<char>& keys = pair.second;
        std::unordered_set<char> uniqueKeys(keys.begin(), keys.end());
        if (uniqueKeys.size() != keys.size()) {
            std::cerr << "Error: Command '" << pair.first
                      << "' has duplicate key bindings" << std::endl;
            return false;
        }
    }
    keyToCommand = std::move(tempKeyToCommand);
    return true;
}

bool KeyboardLayout::loadFromFile(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Warning: Cannot open config file: " << configFile
                  << ". Using default layout." << std::endl;
        return false;
    }

    commandToKeys.clear();
    std::string line;
    int lineNum = 0;

    while (std::getline(file, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue;

        size_t start = line.find_first_not_of(" \t");
        if (start == std::string::npos) continue;
        size_t end = line.find_last_not_of(" \t");
        line = line.substr(start, end - start + 1);

        size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos) {
            std::cerr << "Error (line " << lineNum << "): Invalid syntax, missing '='" << std::endl;
            continue;
        }

        std::string command = line.substr(0, equalsPos);
        std::string keysStr = line.substr(equalsPos + 1);
        command.erase(command.find_last_not_of(" \t") + 1);
        keysStr.erase(0, keysStr.find_first_not_of(" \t"));
        std::vector<char> keys;
        std::stringstream keysStream(keysStr);
        std::string keyToken;
        while (std::getline(keysStream, keyToken, ',')) {
            keyToken.erase(0, keyToken.find_first_not_of(" \t"));
            keyToken.erase(keyToken.find_last_not_of(" \t") + 1);
            char key = parseKey(keyToken);
            if (key != 0) {
                keys.push_back(key);
            } else {
                std::cerr << "Warning (line " << lineNum << "): Invalid key: " << keyToken << std::endl;
            }
        }
        if (!keys.empty()) {
            commandToKeys[command] = keys;
        }
    }
    file.close();
    if (!validateLayout()) {
        std::cerr << "Error: Invalid keyboard layout in config file. Using default layout." << std::endl;
        return false;
    }
    auto getKey = [this](const std::string& command, char defaultValue) {
        if (commandToKeys.find(command) != commandToKeys.end() &&
            !commandToKeys[command].empty()) {
            return commandToKeys[command][0];
        }
        return defaultValue;
    };
    KEYUP = getKey("MOVE_UP", StandardKeyboardLayout::KEYUP);
    KEYLEFT = getKey("MOVE_LEFT", StandardKeyboardLayout::KEYLEFT);
    KEYRIGHT = getKey("MOVE_RIGHT", StandardKeyboardLayout::KEYRIGHT);
    KEYDOWN = getKey("MOVE_DOWN", StandardKeyboardLayout::KEYDOWN);
    KEYE = getKey("USE_ITEM", StandardKeyboardLayout::KEYE);
    KEYQ = getKey("SWAP_WEAPON", StandardKeyboardLayout::KEYQ);
    KEYESC = getKey("MENU", StandardKeyboardLayout::KEYESC);
    KEYENTER = getKey("SELECT", StandardKeyboardLayout::KEYENTER);
    KEY1 = getKey("SPELL_1", StandardKeyboardLayout::KEY1);
    KEY2 = getKey("SPELL_2", StandardKeyboardLayout::KEY2);
    KEY3 = getKey("SPELL_3", StandardKeyboardLayout::KEY3);
    KEY4 = getKey("SPELL_4", StandardKeyboardLayout::KEY4);
    std::cout << "Keyboard layout loaded successfully from: " << configFile << std::endl;
    return true;
}

void KeyboardLayout::printLayout() const {
    for (const auto& pair : commandToKeys) {
        std::cout << pair.first << " = ";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            if (i > 0) std::cout << ", ";
            char key = pair.second[i];
            if (key == 27) std::cout << "ESC";
            else if (key == '\n') std::cout << "ENTER";
            else std::cout << "'" << key << "'";
        }
        std::cout << std::endl;
    }
}

std::string KeyboardLayout::getCommandForKey(char key) const {
    auto it = keyToCommand.find(key);
    if (it != keyToCommand.end()) {
        return it->second;
    }
    return "";
}