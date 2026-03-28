#ifndef PLAYER_TURN_HANDLER_H
#define PLAYER_TURN_HANDLER_H

#include "GameField.h"
#include "EntityManager.h"
#include "SpellFactory.h"
#include "RandomGenerator.h"
#include "ScoreSystem.h"
#include "MovementHelper.h"
#include "SpellShop.h"
#include "TurnResults.h"
#include <functional>

class PlayerTurnHandler {
private:
	GameField& field;
	EntityManager& entity_manager;
	std::reference_wrapper<int> player_index_ref;
	SpellFactory& spell_factory;
	RandomGenerator& rng;
	ScoreSystem& score_system;
	
	MovementHelper movement_helper;
	SpellShop spell_shop;
	
	MoveResult processPlayerMovement(char command);
	AttackResult processPlayerAttack(int target_index);
	SpellCastResult processSpellCasting();

public:
	PlayerTurnHandler(GameField& game_field, EntityManager& em, int& player_idx,
	                  SpellFactory& factory, RandomGenerator& random_gen, ScoreSystem& score_sys);
	
	PlayerTurnResult processPlayerTurn();
	bool isPlayerAlive() const;
};

#endif







