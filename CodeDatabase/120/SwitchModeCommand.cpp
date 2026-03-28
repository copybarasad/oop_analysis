#include "SwitchModeCommand.h"

bool SwitchModeCommand::Execute(GameContext& context) {
	auto& player = context.world.GetPlayer();
	player.SwitchCombatMode();
	std::string mode = (player.GetMode() == Player::CombatMode::Melee) ? "melee" : "ranged";
	context.events.Notify(GameMessageEvent{"Player switched to " + mode + " mode (damage = " + std::to_string(player.GetDamage()) + ")"});
	return false;
}
