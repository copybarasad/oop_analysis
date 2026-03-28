#include "InputMap.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>

InputMap InputMap::load() {
    InputMap m;
    m.loadDefaults();

    std::ifstream f("controls.ini");
    if (!f.is_open()) {
        std::cout << "Controls config not found, using defaults" << std::endl;
        return m;
    }

    m.actionToKey.clear();
    m.keyToAction.clear();

    m.loadDefaults();

    std::string line;
    while (std::getline(f, line)) {
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        val.erase(0, val.find_first_not_of(" \t"));
        val.erase(val.find_last_not_of(" \t") + 1);

        if (val.empty()) continue;

        char inputChar = std::tolower(val[0]);

        Action action = ACTION_COUNT;

        if (key == "up") action = UP;
        else if (key == "down") action = DOWN;
        else if (key == "left") action = LEFT;
        else if (key == "right") action = RIGHT;
        else if (key == "cast") action = CAST;
        else if (key == "attack") action = ATTACK;
        else if (key == "save") action = SAVE;
        else if (key == "quit") action = QUIT;
        else if (key == "spell1") action = SPELL_0;
        else if (key == "spell2") action = SPELL_1;
        else if (key == "spell3") action = SPELL_2;
        else if (key == "spell4") action = SPELL_3;
        else if (key == "spell5") action = SPELL_4;
        else if (key == "menu1") action = MENU_0;
        else if (key == "menu2") action = MENU_1;
        else if (key == "menu3") action = MENU_2;

        if (action != ACTION_COUNT) {
            m.actionToKey[action] = inputChar;
        }
    }

    for (const auto& pair : m.actionToKey) {
        m.keyToAction[pair.second] = pair.first;
    }

    if (m.hasConflicts()) {
        std::cout << "Conflict detected in controls.ini! Using default controls." << std::endl;
        m.resetToDefaults();
    }

    return m;
}

void InputMap::loadDefaults() {
    actionToKey = {
        {UP, 'w'},
        {DOWN, 's'},
        {LEFT, 'a'},
        {RIGHT, 'd'},
        {CAST, 'e'},
        {ATTACK, 'e'},
        {SAVE, 'k'},
        {QUIT, 'q'},
        {SPELL_0, '4'},
        {SPELL_1, '5'},
        {SPELL_2, '6'},
        {SPELL_3, '7'},
        {SPELL_4, '8'},
        {MENU_0, '1'},
        {MENU_1, '2'},
        {MENU_2, '3'}
    };

    // Инициализируем обратное отображение
    for (const auto& pair : actionToKey) {
        keyToAction[pair.second] = pair.first;
    }
}

char InputMap::get(Action action) const {
    auto it = actionToKey.find(action);
    return it != actionToKey.end() ? it->second : 0;
}

char InputMap::getChar(char action) const {
    switch (std::tolower(action)) {
        case 'u': return get(UP);
        case 'd': return get(DOWN);
        case 'l': return get(LEFT);
        case 'r': return get(RIGHT);
        case 'c': return get(CAST);
        default: return action;
    }
}

InputMap::Action InputMap::getAction(char input) const {
    auto it = keyToAction.find(std::tolower(input));
    return it != keyToAction.end() ? it->second : ACTION_COUNT;
}

bool InputMap::isValid() const {
    return !hasConflicts() && actionToKey.size() == ACTION_COUNT;
}

bool InputMap::hasConflicts() const {
    std::vector<char> keys;
    for (const auto& pair : actionToKey) {
        keys.push_back(pair.second);
    }

    std::sort(keys.begin(), keys.end());
    auto last = std::unique(keys.begin(), keys.end());
    return last != keys.end();
}

void InputMap::resetToDefaults() {
    actionToKey.clear();
    keyToAction.clear();
    loadDefaults();
}

// глобальная константа
const InputMap Controls = InputMap::load();