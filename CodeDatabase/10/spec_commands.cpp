#include "spec_commands.h"
#include "game.h"
#include <iostream>

MoveCommand::MoveCommand(int dx, int dy) : dx(dx), dy(dy) {}

bool MoveCommand::execute(Game& game) {
    return game.movePlayer(dx, dy);
}

std::string MoveCommand::getName() const {
    if (dx == 0 && dy == -1) return "MOVE_UP";
    if (dx == 0 && dy == 1) return "MOVE_DOWN";
    if (dx == -1 && dy == 0) return "MOVE_LEFT";
    if (dx == 1 && dy == 0) return "MOVE_RIGHT";
    return "MOVE";
}

bool AttackCommand::execute(Game& game) {
    return game.attack();
}

bool ChangeCombatTypeCommand::execute(Game& game) {
    return game.changeCombatType();
}

bool ShowStatusCommand::execute(Game& game) {
    return game.showStatus();
}

bool ShowSpellsCommand::execute(Game& game) {
    return game.showSpells();
}

CastSpellCommand::CastSpellCommand(int spellIndex, int targetX, int targetY) 
    : spellIndex(spellIndex), targetX(targetX), targetY(targetY) {}

bool CastSpellCommand::execute(Game& game) {
    return game.castSpell(spellIndex, targetX, targetY);
}

bool BuySpellCommand::execute(Game& game) {
    return game.buySpell();
}

bool SaveGameCommand::execute(Game& game) {
    game.saveGame();
    return false;
}

bool LoadGameCommand::execute(Game& game) {
    game.loadGame();
    return false;
}

bool QuitCommand::execute(Game& game) {
    game.setGameRunning(false);
    return false;
}

bool ShowMenuCommand::execute(Game& game) {
    return game.showMenu();
}

bool RestartCommand::execute(Game& game) {
    game.restartGame();
    return false;
}
