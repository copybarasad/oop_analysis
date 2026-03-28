#include "commands.hpp"
#include "Game.hpp"

MoveCommand::MoveCommand(int x, int y) : dx(x), dy(y) {}

void MoveCommand::execute(Game& game) {
    game.movePlayer(dx, dy);
}

void AttackCommand::execute(Game& game) {
    game.playerAttack();
}

void SwitchWeaponCommand::execute(Game& game) {
    game.switchPlayerWeapon();
}

CastSpellCommand::CastSpellCommand(int idx, int tx, int ty) 
    : spellIndex(idx), targetX(tx), targetY(ty) {}

void CastSpellCommand::execute(Game& game) {
    game.castSpell(spellIndex, targetX, targetY);
}

void SaveCommand::execute(Game& game) {
    game.saveGame("savegame.txt");
}

void LoadCommand::execute(Game& game) {
    game.loadGame("savegame.txt");
}

void ExitCommand::execute(Game& game) {
    game.stop();
}

void NullCommand::execute(Game& /*game*/) {
}