#include "OpenShopCommand.h"
#include "AoESpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"

bool OpenShopCommand::Execute(GameContext& context) {
	auto& player = context.world.GetPlayer();
	auto choiceOpt = context.input_reader.GetShopChoice(player.GetScore());
	if (!choiceOpt) return false;

	int choice = *choiceOpt;
	std::unique_ptr<ISpell> spell = nullptr;
	int cost = 0;

	switch (choice) {
		case 1: spell = std::make_unique<DamageSpell>(10,3); cost = 20; break;
		case 2: spell = std::make_unique<AoESpell>(20,2); cost = 35; break;
		case 3: spell = std::make_unique<TrapSpell>(5); cost = 15; break;
		case 4: spell = std::make_unique<SummonSpell>(1); cost = 30; break;
		case 5: spell = std::make_unique<BuffSpell>(); cost = 20; break;
		default: context.events.Notify(GameMessageEvent{"Invalid choice!"}); return false;
	}

	if (player.GetScore() < cost) {
		context.events.Notify(GameMessageEvent{"Not enough points!"});
		return false;
	}

	if (player.GetHand().IsFull()) {
		context.events.Notify(GameMessageEvent{"Hand is full!"});
		return false;
	}

	player.AddScore(-cost);
	player.GetHand().AddSpell(std::move(spell));
	context.events.Notify(GameMessageEvent{"Bought new spell."});
	return false;
}
