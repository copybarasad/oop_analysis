#include "ranged_attack_command.h"
#include "game.h"

void RangedAttackCommand::execute(Game& game) {
    if (game.combatSystem) {
        game.combatSystem->performRangedAttack();
    }
}
