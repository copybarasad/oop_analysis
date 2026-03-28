#include "attack_command.h"
#include "game_session.h"

void AttackCommand::Execute(GameSession& game) {
    game.ProcessAttack();
}
