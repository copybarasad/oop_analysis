#include "AttackCommand.h"

bool AttackCommand::Execute(GameContext& context) {
	context.logic.PlayerAttack();
	return true;
}
