#ifndef GAMELB1_INPUTHANDLER_H
#define GAMELB1_INPUTHANDLER_H

#include <SFML/Window/Event.hpp>
#include <map>
#include <string>
#include <memory>
#include "Command.h"

enum class GameAction {
    MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
    ACTION_COMBAT, ACTION_FIRE,
    SPELL_1, SPELL_2, SPELL_3, SPELL_4, SPELL_5,
    SAVE, LOAD, NEXT_LEVEL, RESTART,
    UPGRADE_1, UPGRADE_2,
    EXIT,
    NONE
};

class InputHandler {
public:
    InputHandler();

    void loadConfig(const std::string& filename);

    std::unique_ptr<Command> handleInput(const sf::Event& event);

private:
    std::map<sf::Keyboard::Key, GameAction> keyMap_;

    void setDefaultControls();
    sf::Keyboard::Key stringToKey(const std::string& s);
};

#endif //GAMELB1_INPUTHANDLER_H