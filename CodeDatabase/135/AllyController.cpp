#include "AllyController.h"
#include "GameField.h"
#include "EntityManager.h"
#include "MovementHelper.h"
#include "IEntity.h"
#include "ICombatant.h"
#include <iostream>

AllyController::AllyController(GameField& game_field, EntityManager& em, MovementHelper& mh, 
                               RandomGenerator& random_gen)
	: field(game_field), entity_manager(em), movement_helper(mh), rng(random_gen) {}

void AllyController::moveAllyTowardsPlayer(int ally_index, int player_index) {
	if (movement_helper.getManhattanDistance(ally_index, player_index) > 2) {
		movement_helper.moveEntityTowards(ally_index, player_index);
	}
}

void AllyController::attackNearbyEnemies(int ally_index, int player_index) {
	(void)player_index;  
	int ally_x, ally_y;
	field.indexToCoord(ally_index, ally_x, ally_y);
	
	using namespace GameConstants::Directions;
	
	for (int i = 0; i < ADJACENT_8_COUNT; ++i) {
		int check_x = ally_x + DX[i];
		int check_y = ally_y + DY[i];
		
		if (!field.isValidPosition(check_x, check_y)) continue;
		
		int check_index = field.coordToIndex(check_x, check_y);
		if (!entity_manager.hasEntity(check_index)) continue;
		
		auto entity_type = entity_manager.getEntityType(check_index);
		if (!entity_type.has_value()) continue;
		
		if (entity_type.value() == EntityType::ENEMY || 
		    entity_type.value() == EntityType::BARRACKS || 
		    entity_type.value() == EntityType::TOWER) {
			
			int damage = 0;
			auto* ally_entity = entity_manager.getEntity(ally_index);
			if (ally_entity) {
				auto* combatant = dynamic_cast<ICombatant*>(ally_entity);
				if (combatant) damage = combatant->getDamage();
			}
			
			entity_manager.causeDamageToEntity(check_index, damage);
			
			std::cout << "  ⚔ Союзник на (" << ally_x << ", " << ally_y << ") атакует врага на (" 
			          << check_x << ", " << check_y << ")!\n";
			std::cout << "    Нанесено урона: " << damage << "\n";
			
			if (!entity_manager.isEntityAlive(check_index)) {
				std::cout << "    ★ ВРАГ УНИЧТОЖЕН! ★\n";
				entity_manager.removeEntity(check_index);
			}
			
			break;
		}
	}
}

void AllyController::processAllyTurn(int player_index) {
	auto ally_indexes = entity_manager.findEntitiesByType(EntityType::ALLY);
	
	if (ally_indexes.empty()) {
		return;
	}
	
	std::cout << "\n--- ДЕЙСТВИЯ СОЮЗНИКОВ ---\n";
	
	std::vector<int> initial_allies = ally_indexes;
	for (int ally_index : initial_allies) {
		if (entity_manager.hasEntity(ally_index)) {
			auto entity_type = entity_manager.getEntityType(ally_index);
			if (entity_type.has_value() && entity_type.value() == EntityType::ALLY) {
				moveAllyTowardsPlayer(ally_index, player_index);
			}
		}
	}
	
	auto updated_ally_indexes = entity_manager.findEntitiesByType(EntityType::ALLY);
	for (int ally_index : updated_ally_indexes) {
		attackNearbyEnemies(ally_index, player_index);
	}
}
