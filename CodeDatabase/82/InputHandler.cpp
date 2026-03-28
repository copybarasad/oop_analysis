#include "InputHandler.h"
#include <iostream>
#include <string>

GameAction InputHandler::getNextAction() const {
    std::string input;
    std::getline(std::cin, input);
    if (input.empty()) {
        return GameAction::INVALID_ACTION;
    }
    char firstChar = input[0];
    switch(firstChar) {
        case 'w': case 'W': return GameAction::MOVE_UP;
        case 's': case 'S': return GameAction::MOVE_DOWN;
        case 'a': case 'A': return GameAction::MOVE_LEFT;
        case 'd': case 'D': return GameAction::MOVE_RIGHT;
        case ' ': return GameAction::SWITCH_COMBAT_MODE;
        case 'f': case 'F': return GameAction::RANGED_ATTACK;
        case 'q': case 'Q': return GameAction::QUIT_GAME;
        case '1': return GameAction::CAST_SPELL_1;
        case '2': return GameAction::CAST_SPELL_2;
        case '3': return GameAction::CAST_SPELL_3;
        case 'g': case 'G': return GameAction::SAVE_GAME;
        default: return GameAction::INVALID_ACTION;
    }
}

Position InputHandler::calculateMoveDirection(GameAction action) const {
    switch(action) {
        case GameAction::MOVE_UP: return Position(0, -1);
        case GameAction::MOVE_DOWN: return Position(0, 1);
        case GameAction::MOVE_LEFT: return Position(-1, 0);
        case GameAction::MOVE_RIGHT: return Position(1, 0);
        default: return Position(0, 0);
    }
}