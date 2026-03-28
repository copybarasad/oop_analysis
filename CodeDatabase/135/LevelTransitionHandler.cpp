#include "LevelTransitionHandler.h"
#include "GameField.h"
#include "EntityManager.h"
#include "LevelDirector.h"
#include "ScoreSystem.h"
#include "GameRenderer.h"
#include "UpgradeSystem.h"
#include "UpgradeMenu.h"
#include "SpellFactory.h"
#include "RandomGenerator.h"
#include "Player.h"
#include "IEntity.h"
#include <iostream>

LevelTransitionHandler::LevelTransitionHandler(GameField& f, EntityManager& em, int& player_idx,
                                               LevelDirector& ld, ScoreSystem& ss, GameRenderer& rend,
                                               UpgradeSystem& us, UpgradeMenu& um, SpellFactory& sf,
                                               RandomGenerator& random_gen)
	: field(f), entity_manager(em), player_index(player_idx), 
	  level_director(ld), score_system(ss), renderer(rend),
	  upgrade_system(us), upgrade_menu(um), spell_factory(sf), rng(random_gen) {}

LevelTransitionHandler::PlayerState LevelTransitionHandler::savePlayerState() {
	PlayerState state = {10, 10, 10, 100, true, 5};
	
	IEntity* entity = entity_manager.getEntity(player_index);
	if (entity) {
		Player* player = dynamic_cast<Player*>(entity);
		if (player) {
		state.intelligence = player->getInt();
		state.dexterity = player->getDex();
		state.strength = player->getStr();
		state.max_hp = player->getHealthStatus().getMaxHealth();
		state.is_melee = player->melee();
			state.hand_size = player->getSpellHand().getMaxSize();
		}
	}
	
	return state;
}

void LevelTransitionHandler::restorePlayerState(const PlayerState& state) {
	IEntity* entity = entity_manager.getEntity(player_index);
	if (!entity) return;
	
	Player* player = dynamic_cast<Player*>(entity);
	if (!player) return;
	
	player->improveIntelligence(state.intelligence - 10);
	player->improveDexterity(state.dexterity - 10);
	player->improveStrength(state.strength - 10);
	
	if (state.max_hp > 100) {
		player->increaseMaxHealth(state.max_hp - 100);
	}
	
	if (!state.is_melee && player->melee()) {
		player->setWeapon();
	}
	
	while (player->getSpellHand().getCurrentSize() < state.hand_size && 
	       player->getSpellHand().hasSpace()) {
		player->getSpellHand().addSpell(spell_factory.createRandomSpell());
	}
}

void LevelTransitionHandler::transitionToNextLevel() {
	
	renderer.showLevelCompleteScreen(score_system.getScore(), score_system.getThreshold());
	std::cout << "\nНажмите Enter для продолжения...\n";
	std::cin.get();
	
	IEntity* entity = entity_manager.getEntity(player_index);
	if (entity) {
		Player* player = dynamic_cast<Player*>(entity);
		if (player) {
			upgrade_menu.showUpgradeScreen(*player, upgrade_system);
			level_director.preparePlayerForNextLevel(*player);
		}
	}
	
	PlayerState saved_state = savePlayerState();
	
	level_director.advanceLevel();
	
	entity_manager = EntityManager();
	player_index = -1;
	
	auto init_result = level_director.initializeLevel(level_director.getCurrentLevel());
	field = std::move(init_result.field);
	entity_manager = std::move(init_result.entity_manager);
	player_index = init_result.player_index;
	score_system.setThreshold(init_result.score_threshold);
	score_system.resetScore();
	
	restorePlayerState(saved_state);
}
