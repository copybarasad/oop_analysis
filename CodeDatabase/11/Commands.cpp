#include "Commands.h"
#include "GameController.h"

MoveCommand::MoveCommand(int _dx, int _dy) : dx(_dx), dy(_dy) {}

void MoveCommand::execute(GameController& game) {
    game.handlePlayerMove(dx, dy);
}

void SwitchModeCommand::execute(GameController& game) {
    game.handleSwitchMode();
}

void CastSpellCommand::execute(GameController& game) {
    game.handleCastSpell();
}

void BuySpellCommand::execute(GameController& game) {
    game.handleBuySpell();
}

void SaveCommand::execute(GameController& game) {
    game.handleSave();
}

void LoadCommand::execute(GameController& game) {
    game.handleLoad();
}

void ExitCommand::execute(GameController& game) {
    game.handleExit();
}