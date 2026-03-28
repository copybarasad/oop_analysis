#include "InputHandler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

InputHandler::InputHandler() {
    setDefaultControls();
}

void InputHandler::setDefaultControls() {
    keyMap_.clear();
    keyMap_[sf::Keyboard::W] = GameAction::MOVE_UP;
    keyMap_[sf::Keyboard::S] = GameAction::MOVE_DOWN;
    keyMap_[sf::Keyboard::A] = GameAction::MOVE_LEFT;
    keyMap_[sf::Keyboard::D] = GameAction::MOVE_RIGHT;
    keyMap_[sf::Keyboard::C] = GameAction::ACTION_COMBAT;
    keyMap_[sf::Keyboard::F] = GameAction::ACTION_FIRE;
    keyMap_[sf::Keyboard::Num1] = GameAction::SPELL_1;
    keyMap_[sf::Keyboard::Num2] = GameAction::SPELL_2;
    keyMap_[sf::Keyboard::Num3] = GameAction::SPELL_3;
    keyMap_[sf::Keyboard::Num4] = GameAction::SPELL_4;
    keyMap_[sf::Keyboard::Num5] = GameAction::SPELL_5;
    keyMap_[sf::Keyboard::F5] = GameAction::SAVE;
    keyMap_[sf::Keyboard::F6] = GameAction::LOAD;
    keyMap_[sf::Keyboard::N] = GameAction::NEXT_LEVEL;
    keyMap_[sf::Keyboard::R] = GameAction::RESTART;
    keyMap_[sf::Keyboard::Q] = GameAction::EXIT;
}

std::unique_ptr<Command> InputHandler::handleInput(const sf::Event& event) {
    if (event.type != sf::Event::KeyPressed) return nullptr;

    auto it = keyMap_.find(event.key.code);
    if (it == keyMap_.end()) return nullptr;

    switch (it->second) {
        case GameAction::MOVE_UP:    return std::make_unique<MoveCommand>('w');
        case GameAction::MOVE_DOWN:  return std::make_unique<MoveCommand>('s');
        case GameAction::MOVE_LEFT:  return std::make_unique<MoveCommand>('a');
        case GameAction::MOVE_RIGHT: return std::make_unique<MoveCommand>('d');

        case GameAction::ACTION_COMBAT: return std::make_unique<ActionCommand>('c');
        case GameAction::ACTION_FIRE:   return std::make_unique<ActionCommand>('f');

        case GameAction::SPELL_1: return std::make_unique<SelectSpellCommand>(0);
        case GameAction::SPELL_2: return std::make_unique<SelectSpellCommand>(1);
        case GameAction::SPELL_3: return std::make_unique<SelectSpellCommand>(2);
        case GameAction::SPELL_4: return std::make_unique<SelectSpellCommand>(3);
        case GameAction::SPELL_5: return std::make_unique<SelectSpellCommand>(4);

        case GameAction::SAVE: return std::make_unique<SaveCommand>();
        case GameAction::LOAD: return std::make_unique<LoadCommand>();
        case GameAction::NEXT_LEVEL: return std::make_unique<NextLevelCommand>();
        case GameAction::RESTART: return std::make_unique<RestartCommand>();


        default: return nullptr;
    }
}

sf::Keyboard::Key InputHandler::stringToKey(const std::string& s) {
    if (s == "W") return sf::Keyboard::W;
    if (s == "U") return sf::Keyboard::U;
    if (s == "S") return sf::Keyboard::S;
    if (s == "A") return sf::Keyboard::A;
    if (s == "D") return sf::Keyboard::D;
    if (s == "C") return sf::Keyboard::C;
    if (s == "F") return sf::Keyboard::F;
    if (s == "1") return sf::Keyboard::Num1;
    if (s == "2") return sf::Keyboard::Num2;
    if (s == "3") return sf::Keyboard::Num3;
    if (s == "4") return sf::Keyboard::Num4;
    if (s == "5") return sf::Keyboard::Num5;
    if (s == "F5") return sf::Keyboard::F5;
    if (s == "F6") return sf::Keyboard::F6;
    if (s == "N") return sf::Keyboard::N;
    if (s == "R") return sf::Keyboard::R;
    if (s == "Q") return sf::Keyboard::Q;
    return sf::Keyboard::Unknown;
}

void InputHandler::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Config file not found, using defaults." << std::endl;
        return;
    }

    std::map<sf::Keyboard::Key, GameAction> newMap;
    std::set<sf::Keyboard::Key> usedKeys;
    std::set<int> usedActions;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string actionStr, keyStr;
        ss >> actionStr >> keyStr;

        GameAction action = GameAction::NONE;
        if (actionStr == "MOVE_UP") action = GameAction::MOVE_UP;
        else if (actionStr == "MOVE_DOWN") action = GameAction::MOVE_DOWN;
        else if (actionStr == "MOVE_LEFT") action = GameAction::MOVE_LEFT;
        else if (actionStr == "MOVE_RIGHT") action = GameAction::MOVE_RIGHT;
        else if (actionStr == "COMBAT") action = GameAction::ACTION_COMBAT;
        else if (actionStr == "FIRE") action = GameAction::ACTION_FIRE;
        else if (actionStr == "SPELL_1") action = GameAction::SPELL_1;
        else if (actionStr == "SPELL_2") action = GameAction::SPELL_2;
        else if (actionStr == "SPELL_3") action = GameAction::SPELL_3;
        else if (actionStr == "SPELL_4") action = GameAction::SPELL_4;
        else if (actionStr == "SPELL_5") action = GameAction::SPELL_5;
        else if (actionStr == "SAVE") action = GameAction::SAVE;
        else if (actionStr == "LOAD") action = GameAction::LOAD;
        else if (actionStr == "NEXT") action = GameAction::NEXT_LEVEL;
        else if (actionStr == "RESTART") action = GameAction::RESTART;
        else if (actionStr == "EXIT") action = GameAction::EXIT;

        if (action == GameAction::NONE) continue;

        sf::Keyboard::Key key = stringToKey(keyStr);
        if (key == sf::Keyboard::Unknown) {
            std::cerr << "Unknown key in config: " << keyStr << ". Using defaults." << std::endl;
            setDefaultControls();
            return;
        }

        if (usedKeys.count(key) || usedActions.count((int)action)) {
            std::cerr << "Duplicate key or action in config. Using defaults." << std::endl;
            setDefaultControls();
            return;
        }

        newMap[key] = action;
        usedKeys.insert(key);
        usedActions.insert((int)action);
    }

    if (!newMap.empty()) {
        keyMap_ = newMap;
        std::cout << "Controls loaded from " << filename << std::endl;
    }
}