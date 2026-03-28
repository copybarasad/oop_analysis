#include "../include/Command.h"
#include "../include/Game.h"
#include "../include/GameEvent.h"
#include <sstream>

std::string MoveCommand::getDescription() const {
    std::string dirStr;
    switch (direction) {
        case Direction::UP: dirStr = "up"; break;
        case Direction::DOWN: dirStr = "down"; break;
        case Direction::LEFT: dirStr = "left"; break;
        case Direction::RIGHT: dirStr = "right"; break;
        default: dirStr = "none"; break;
    }
    return "Move " + dirStr;
}

bool MoveCommand::execute(Game& game) {
    return game.executeMove(direction);
}

std::string AttackCommand::getDescription() const {
    std::string dirStr;
    switch (direction) {
        case Direction::UP: dirStr = "up"; break;
        case Direction::DOWN: dirStr = "down"; break;
        case Direction::LEFT: dirStr = "left"; break;
        case Direction::RIGHT: dirStr = "right"; break;
        default: dirStr = "none"; break;
    }
    return "Attack " + dirStr;
}

bool AttackCommand::execute(Game& game) {
    return game.executeAttack(direction);
}

bool SwitchModeCommand::execute(Game& game) {
    return game.executeSwitchMode();
}

bool SkipTurnCommand::execute(Game& game) {
    return game.executeSkipTurn();
}

std::string CastSpellCommand::getDescription() const {
    std::ostringstream oss;
    oss << "Cast spell #" << spellIndex << " at (" 
        << targetPosition.getX() << "," << targetPosition.getY() << ")";
    return oss.str();
}

bool CastSpellCommand::execute(Game& game) {
    return game.executeCastSpell(spellIndex, targetPosition);
}

bool BuySpellCommand::execute(Game& game) {
    return game.executeBuySpell();
}

bool SaveGameCommand::execute(Game& game) {
    return game.executeSaveGame();
}

bool RestartCommand::execute(Game& game) {
    return game.executeRestart();
}

bool QuitCommand::execute(Game& game) {
    return game.executeQuit();
}

bool InvalidCommand::execute(Game& game) {
    (void)game;
    return false;
}
