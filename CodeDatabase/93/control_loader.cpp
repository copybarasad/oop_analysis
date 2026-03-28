#include "control_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>

ControlMapping ControlLoader::loadFromFile(const std::string& filename) {
    ControlMapping mapping;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Controls file not found: " << filename << ". Using defaults.\n";
        return ControlMapping::defaultMapping();
    }

    std::string line;
    int lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        if (line.empty() || line[0] == '#' || line[0] == ';') 
            continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) 
            continue;

        auto trim = [](std::string s) {
            auto not_space = [](int c) { return !std::isspace(c); };
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
            s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
            return s;
        };

        std::string keyName = trim(line.substr(0, eq));
        std::string val = trim(line.substr(eq + 1));

        if (val.length() != 1) 
            continue;
        char key = val[0];

        CommandId cmd;
        if (keyName == "move_up") cmd = CommandId::MOVE_UP;
        else if (keyName == "move_down") cmd = CommandId::MOVE_DOWN;
        else if (keyName == "move_left") cmd = CommandId::MOVE_LEFT;
        else if (keyName == "move_right") cmd = CommandId::MOVE_RIGHT;
        else if (keyName == "attack_up") cmd = CommandId::ATTACK_UP;
        else if (keyName == "attack_down") cmd = CommandId::ATTACK_DOWN;
        else if (keyName == "attack_left") cmd = CommandId::ATTACK_LEFT;
        else if (keyName == "attack_right") cmd = CommandId::ATTACK_RIGHT;
        else if (keyName == "switch_mode") cmd = CommandId::SWITCH_MODE;
        else if (keyName == "buy_spell") cmd = CommandId::BUY_SPELL;
        else if (keyName == "use_spell_1") cmd = CommandId::USE_SPELL_1;
        else if (keyName == "use_spell_2") cmd = CommandId::USE_SPELL_2;
        else if (keyName == "use_spell_3") cmd = CommandId::USE_SPELL_3;
        else if (keyName == "save_game") cmd = CommandId::SAVE_GAME;
        else if (keyName == "exit_game") cmd = CommandId::EXIT_GAME;
        else 
            continue;

        mapping.keyToCommand[key] = cmd;
    }

    mapping.commandToKey.clear();
    for (const auto& pair : mapping.keyToCommand) {
        mapping.commandToKey[pair.second] = pair.first;
    }

    file.close();

    if (!mapping.isValid()) {
        std::cerr << "Invalid control mapping in " << filename << ". Using defaults.\n";
        return ControlMapping::defaultMapping();
    }

    std::cout << "Controls loaded from " << filename << "\n";
    return mapping;
}

void ControlLoader::saveDefaultToFile(const std::string& filename) {
    ControlMapping def = ControlMapping::defaultMapping();
    std::ofstream file(filename);
    if (!file.is_open()) 
        return;

    auto writeEntry = [&](const char* name, CommandId cmd) {
        auto it = def.commandToKey.find(cmd);
        if (it != def.commandToKey.end()) {
            file << name << "=" << it->second << "\n";
        }
    };

    writeEntry("move_up", CommandId::MOVE_UP);
    writeEntry("move_down", CommandId::MOVE_DOWN);
    writeEntry("move_left", CommandId::MOVE_LEFT);
    writeEntry("move_right", CommandId::MOVE_RIGHT);
    writeEntry("attack_up", CommandId::ATTACK_UP);
    writeEntry("attack_down", CommandId::ATTACK_DOWN);
    writeEntry("attack_left", CommandId::ATTACK_LEFT);
    writeEntry("attack_right", CommandId::ATTACK_RIGHT);
    writeEntry("switch_mode", CommandId::SWITCH_MODE);
    writeEntry("buy_spell", CommandId::BUY_SPELL);
    writeEntry("use_spell_1", CommandId::USE_SPELL_1);
    writeEntry("use_spell_2", CommandId::USE_SPELL_2);
    writeEntry("use_spell_3", CommandId::USE_SPELL_3);
    writeEntry("save_game", CommandId::SAVE_GAME);
    writeEntry("exit_game", CommandId::EXIT_GAME);

    file.close();
    std::cout << "Default controls saved to " << filename << "\n";
}