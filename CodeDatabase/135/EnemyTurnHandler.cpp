#include "EnemyTurnHandler.h"
#include "Enemy.h"
#include "EnemyBarracks.h"
#include "ICombatant.h"
#include "GameConstants.h"
#include "IEntity.h"

EnemyTurnHandler::EnemyTurnHandler(GameField& game_field, EntityManager& em, int& player_idx, RandomGenerator& random_gen)
	: field(game_field),
	  entity_manager(em),
	  player_index_ref(player_idx),
	  rng(random_gen),
	  movement_helper(game_field, em) {}

EnemyAction EnemyTurnHandler::moveEnemyTowardsPlayer(int enemy_index, int player_index) {
	EnemyAction action;
	action.moved = false;
	action.attacked = false;
	
	field.indexToCoord(enemy_index, action.enemy_x, action.enemy_y);
	action.new_x = action.enemy_x;
	action.new_y = action.enemy_y;
	
	if (movement_helper.isAdjacent(enemy_index, player_index)) {
		if (entity_manager.hasEntity(enemy_index) && entity_manager.hasEntity(player_index)) {
			action.attacked = true;
			
			auto* enemy_entity = entity_manager.getEntity(enemy_index);
			if (enemy_entity) {
				auto* combatant = dynamic_cast<ICombatant*>(enemy_entity);
				if (combatant) action.damage_dealt = combatant->getDamage();
			}
			
			auto player_health = entity_manager.getEntityHealth(player_index);
			if (player_health.has_value()) {
				auto [player_hp_before, player_max_hp] = player_health.value();
				action.player_hp_before = player_hp_before;
				action.player_max_hp = player_max_hp;
				
				entity_manager.causeDamageToEntity(player_index, action.damage_dealt);
				
				auto [player_hp_after, _] = entity_manager.getEntityHealth(player_index).value_or(std::make_pair(0, 0));
				action.player_hp_after = player_hp_after;
			}
		}
		return action;
	}
	
	if (movement_helper.moveEntityTowards(enemy_index, player_index)) {
		action.moved = true;
		auto enemy_indexes = entity_manager.findEntitiesByType(EntityType::ENEMY);
		for (int idx : enemy_indexes) {
			if (idx == enemy_index) continue;
			int new_x, new_y;
			field.indexToCoord(idx, new_x, new_y);
			if (new_x != action.enemy_x || new_y != action.enemy_y) {
				action.new_x = new_x;
				action.new_y = new_y;
				break;
			}
		}
	}
	
	return action;
}

SpawnEvent EnemyTurnHandler::spawnEnemyNearBarracks(int barracks_index) {
	SpawnEvent event;
	event.success = false;
	
	field.indexToCoord(barracks_index, event.barracks_x, event.barracks_y);
	
	int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
	int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};
	
	for (int i = 0; i < 8; ++i) {
		int new_x = event.barracks_x + dx[i];
		int new_y = event.barracks_y + dy[i];
		
		if (field.isValidPosition(new_x, new_y)) {
			int new_index = field.coordToIndex(new_x, new_y);
			if (field.getCell(new_index).isPassable() && !entity_manager.hasEntity(new_index)) {
				auto enemy = std::make_unique<Enemy>(GameConstants::ENEMY_ATTACK, GameConstants::ENEMY_HP);
				entity_manager.addEntity(std::move(enemy), new_index);
				
				event.spawned_x = new_x;
				event.spawned_y = new_y;
				event.success = true;
				return event;
			}
		}
	}
	
	event.message = "нет свободного места";
	return event;
}

EnemyTurnResult EnemyTurnHandler::processEnemyTurn() {
	int& player_index = player_index_ref.get();
	EnemyTurnResult result;
	
	if (player_index < 0 || !entity_manager.hasEntity(player_index)) {
		return result;
	}
	
	auto enemy_indexes = entity_manager.findEntitiesByType(EntityType::ENEMY);
	
	for (int enemy_index : enemy_indexes) {
		EnemyAction action = moveEnemyTowardsPlayer(enemy_index, player_index);
		result.actions.push_back(action);
	}
	
	return result;
}
