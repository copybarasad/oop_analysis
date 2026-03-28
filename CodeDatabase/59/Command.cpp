#include "Command.h"
#include "Game.h"

void MoveCommand::execute(Game& game) {
    game.processMovement(dx, dy);
}

void ToggleWeaponCommand::execute(Game& game) {
    game.togglePlayerWeapon();
}

void ShopCommand::execute(Game& game) {
    game.processShop();
}

void CastSpellCommand::execute(Game& game) {
    game.processSpellCast();
}

void SaveCommand::execute(Game& game) {
    game.processSave();
}

void QuitCommand::execute(Game& game) {
    game.setGameState(Game::GameState::Player_quit);
}