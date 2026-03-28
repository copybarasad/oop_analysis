#include "UpdateHandler.h"
#include "EnemyBarracks.h"
#include "Enemy.h"
#include "ISpawner.h"
#include "GameConstants.h"
#include "IEntity.h"

UpdateHandler::UpdateHandler(GameField& game_field, EntityManager& em, int& player_idx)
	: field(game_field),
	  entity_manager(em),
	  player_index_ref(player_idx),
	  trap_manager(game_field, em) {}

UpdateResult UpdateHandler::update() {
	UpdateResult result;
	result.entities_removed = 0;
	
	auto barracks_indexes = entity_manager.findEntitiesByType(EntityType::BARRACKS);
	for (int barracks_index : barracks_indexes) {
		auto* entity = entity_manager.getEntity(barracks_index);
		if (entity) {
			auto* barracks = dynamic_cast<EnemyBarracks*>(entity);
			if (barracks) {
				if (barracks->update()) {
					
					SpawnEvent spawn;
					spawn.success = false;
					
					field.indexToCoord(barracks_index, spawn.barracks_x, spawn.barracks_y);
					
					int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
					int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};
					
					for (int i = 0; i < 8; ++i) {
						int new_x = spawn.barracks_x + dx[i];
						int new_y = spawn.barracks_y + dy[i];
						
						if (field.isValidPosition(new_x, new_y)) {
							int new_index = field.coordToIndex(new_x, new_y);
							if (field.getCell(new_index).isPassable() && !entity_manager.hasEntity(new_index)) {
								auto enemy = std::make_unique<Enemy>(GameConstants::ENEMY_ATTACK, GameConstants::ENEMY_HP);
								entity_manager.addEntity(std::move(enemy), new_index);
								
								spawn.spawned_x = new_x;
								spawn.spawned_y = new_y;
								spawn.success = true;
								result.spawns.push_back(spawn);
								break;
							}
						}
					}
					
					if (!spawn.success) {
						spawn.message = "нет свободного места";
						result.spawns.push_back(spawn);
					}
				}
			}
		}
	}
	
	std::vector<int> to_remove;
	int field_size = field.getWidth() * field.getHeight();
	for (int i = 0; i < field_size; ++i) {
		if (entity_manager.hasEntity(i) && !entity_manager.isEntityAlive(i)) {
			to_remove.push_back(i);
		}
	}
	
	for (int index : to_remove) {
		entity_manager.removeEntity(index);
	}
	
	result.entities_removed = to_remove.size();
	
	return result;
}
