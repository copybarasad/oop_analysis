#include "console_input.h"
ConsoleInput::ConsoleInput() {
    if (!loadConfig("keys.cfg")) {
        std::cerr << "[Warning] Ошибка конфигурации или файл не найден. Загружены настройки по умолчанию." << std::endl;
        loadDefaults();
    }
}
GameCommand ConsoleInput::parseCommandString(const std::string& cmdStr) {
    GameCommand cmd;
    cmd.type = CommandType::None;
    if (cmdStr == "MOVE_UP") { cmd.type = CommandType::Move; cmd.y = -1; }
    else if (cmdStr == "MOVE_DOWN") { cmd.type = CommandType::Move; cmd.y = 1; }
    else if (cmdStr == "MOVE_LEFT") { cmd.type = CommandType::Move; cmd.x = -1; }
    else if (cmdStr == "MOVE_RIGHT") { cmd.type = CommandType::Move; cmd.x = 1; }
    else if (cmdStr == "ATTACK") { cmd.type = CommandType::Attack; }
    else if (cmdStr == "SWITCH_MODE") { cmd.type = CommandType::SwitchMode; }
    else if (cmdStr == "CAST_SPELL") { cmd.type = CommandType::CastSpell; cmd.param = 0; }
    else if (cmdStr == "BUY_SPELL") { cmd.type = CommandType::BuySpell; }
    else if (cmdStr == "SAVE") { cmd.type = CommandType::Save; }
    else if (cmdStr == "LOAD") { cmd.type = CommandType::Load; }
    else if (cmdStr == "QUIT") { cmd.type = CommandType::Quit; }
    return cmd;
}
void ConsoleInput::loadDefaults() {
    keyMap.clear();
    keyMap["w"] = parseCommandString("MOVE_UP");
    keyMap["s"] = parseCommandString("MOVE_DOWN");
    keyMap["a"] = parseCommandString("MOVE_LEFT");
    keyMap["d"] = parseCommandString("MOVE_RIGHT");
    keyMap["f"] = parseCommandString("ATTACK");
    keyMap["q"] = parseCommandString("SWITCH_MODE");
    keyMap["u"] = parseCommandString("CAST_SPELL");
    keyMap["m"] = parseCommandString("BUY_SPELL");
    keyMap["save"] = parseCommandString("SAVE");
    keyMap["load"] = parseCommandString("LOAD");
    keyMap["quit"] = parseCommandString("QUIT");
}
bool ConsoleInput::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::map<std::string, GameCommand> tempMap;
    std::set<std::string> usedKeys;
    std::set<std::string> usedCommands;
    std::string key, cmdStr;
    while (file >> key >> cmdStr) {
        if (usedKeys.count(key)) {
            return false;
        }
        if (usedCommands.count(cmdStr)) {
            return false;
        }
        GameCommand cmd = parseCommandString(cmdStr);
        if (cmd.type == CommandType::None) {
            return false;
        }
        tempMap[key] = cmd;
        usedKeys.insert(key);
        usedCommands.insert(cmdStr);
    }
    for (const auto& req : requiredCommands) {
        if (usedCommands.find(req) == usedCommands.end()) {
            return false;
        }
    }
    keyMap = tempMap;
    return true;
}
GameCommand ConsoleInput::getCommand() {
    std::string input;
    std::cin >> input;
    if (keyMap.count(input)) {
        return keyMap[input];
    }
    GameCommand empty;
    empty.type = CommandType::None;
    return empty;
}