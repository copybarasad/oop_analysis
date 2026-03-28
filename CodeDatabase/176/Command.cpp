#include "Command.h"
#include "GameController.h"
#include <iostream>

MoveCommand::MoveCommand(Direction dir) : direction(dir) {}

void MoveCommand::execute(GameController& controller) const {
    controller.processPlayerAction(direction);
}

std::string MoveCommand::getDescription() const {
    switch (direction) {
        case Direction::UP: return "Move player up";
        case Direction::DOWN: return "Move player down";
        case Direction::LEFT: return "Move player left";
        case Direction::RIGHT: return "Move player right";
        default: return "Invalid move command";
    }
}

AttackCommand::AttackCommand(Direction dir) : direction(dir) {}

void AttackCommand::execute(GameController& controller) const {
    controller.processAttack(direction);
}

std::string AttackCommand::getDescription() const {
    switch (direction) {
        case Direction::UP: return "Attack up";
        case Direction::DOWN: return "Attack down";
        case Direction::LEFT: return "Attack left";
        case Direction::RIGHT: return "Attack right";
        default: return "Invalid attack command";
    }
}

void SwitchModeCommand::execute(GameController& controller) const {
    controller.processCombatModeSwitch();
}

std::string SwitchModeCommand::getDescription() const {
    return "Switch combat mode";
}

CastSpellCommand::CastSpellCommand(int index, int x, int y)
    : spellIndex(index), targetX(x), targetY(y) {}

void CastSpellCommand::execute(GameController& controller) const {
    controller.processSpellCast(spellIndex, targetX, targetY);
}

std::string CastSpellCommand::getDescription() const {
    return "Cast spell " + std::to_string(spellIndex + 1) +
           " at (" + std::to_string(targetX) + ", " + std::to_string(targetY) + ")";
}

void PurchaseSpellCommand::execute(GameController& controller) const {
    controller.processSpellPurchase();
}

std::string PurchaseSpellCommand::getDescription() const {
    return "Purchase new spell";
}

void SaveGameCommand::execute(GameController& controller) const {
    controller.saveGame();
}

std::string SaveGameCommand::getDescription() const {
    return "Save game";
}

void QuitCommand::execute(GameController& controller) const {
}

std::string QuitCommand::getDescription() const {
    return "Quit to main menu";
}
