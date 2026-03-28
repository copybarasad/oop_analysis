#include "TowerController.h"
#include "GameField.h"
#include "EntityManager.h"
#include "MovementHelper.h"
#include "IEntity.h"
#include "ICooldownable.h"
#include "GameConstants.h"
#include <iostream>

TowerController::TowerController(GameField& game_field, EntityManager& em, MovementHelper& mh)
	: field(game_field), entity_manager(em), movement_helper(mh) {}

int TowerController::getTowerDamage(int tower_index) const {
	return entity_manager.getEntityDamage(tower_index).value_or(0);
}

bool TowerController::tryAttackPlayer(int tower_index, int player_index) {
	int range = GameConstants::TOWER_ATTACK_RANGE;
	int distance = movement_helper.getManhattanDistance(tower_index, player_index);
	
	if (distance <= range) {
		int damage = getTowerDamage(tower_index);
		entity_manager.causeDamageToEntity(player_index, damage);
		
		int tower_x, tower_y;
		field.indexToCoord(tower_index, tower_x, tower_y);
		
		std::cout << "  🏯 Башня на (" << tower_x << ", " << tower_y << ") атакует игрока!\n";
		std::cout << "    Нанесено урона: " << damage << "\n";
		
		return true;
	}
	return false;
}

bool TowerController::tryAttackAllies(int tower_index) {
	int range = GameConstants::TOWER_ATTACK_RANGE;
	
	for (int i = 0; i < field.getWidth() * field.getHeight(); ++i) {
		if (entity_manager.hasEntity(i)) {
			auto entity_type = entity_manager.getEntityType(i);
			if (entity_type.has_value() && entity_type.value() == EntityType::ALLY) {
				int distance = movement_helper.getManhattanDistance(tower_index, i);
				if (distance <= range) {
					int damage = getTowerDamage(tower_index);
					entity_manager.causeDamageToEntity(i, damage);
					
					int tower_x, tower_y;
					field.indexToCoord(tower_index, tower_x, tower_y);
					int ally_x, ally_y;
					field.indexToCoord(i, ally_x, ally_y);
					
					std::cout << "  🏯 Башня на (" << tower_x << ", " << tower_y 
					          << ") атакует союзника на (" << ally_x << ", " << ally_y << ")!\n";
					std::cout << "    Нанесено урона: " << damage << "\n";
					
					if (!entity_manager.isEntityAlive(i)) {
						std::cout << "    ☠ Союзник убит!\n";
						entity_manager.removeEntity(i);
					}
					
					return true;
				}
			}
		}
	}
	return false;
}

void TowerController::processTowerAttacks(int player_index) {
	auto tower_indexes = entity_manager.findEntitiesByType(EntityType::TOWER);
	
	for (int tower_index : tower_indexes) {
		auto* entity = entity_manager.getEntity(tower_index);
		if (entity) {
			auto* cooldownable = dynamic_cast<ICooldownable*>(entity);
			if (cooldownable) {
				if (cooldownable->canAttack()) {
					bool attacked = tryAttackPlayer(tower_index, player_index);
					if (!attacked) {
						attacked = tryAttackAllies(tower_index);
					}
					if (attacked) {
						cooldownable->resetCooldown();
					}
				}
			}
		}
	}
}
