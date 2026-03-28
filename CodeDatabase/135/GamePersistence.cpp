#include "GamePersistence.h"
#include "Player.h"
#include "GameException.h"
#include "IEntity.h"
#include "ICombatant.h"
#include <memory>

GamePersistence::GamePersistence(GameState& state, GameSystems& systems,
                                   GameStateManager& manager, ScoreSystem& score,
                                   LevelDirector& director)
	: game_state(state), game_systems(systems), state_manager(manager),
	  score_system(score), level_director(director) {}

Player* GamePersistence::getPlayer() {
	int player_idx = game_state.getPlayerIndex();
	if (player_idx < 0) {
		return nullptr;
	}
	IEntity* entity = game_state.getEntityManager().getEntity(player_idx);
	return dynamic_cast<Player*>(entity);
}

const Player* GamePersistence::getPlayer() const {
	int player_idx = game_state.getPlayerIndex();
	if (player_idx < 0) {
		return nullptr;
	}
	const IEntity* entity = game_state.getEntityManager().getEntity(player_idx);
	return dynamic_cast<const Player*>(entity);
}

std::vector<EntityData> GamePersistence::collectAllEntities() const {
	std::vector<EntityData> entities;
	
	auto enemy_indices = game_state.getEntityManager().findEntitiesByType(EntityType::ENEMY);
	for (int index : enemy_indices) {
		const IEntity* entity = game_state.getEntityManager().getEntity(index);
		if (entity && entity->alive()) {
			auto health = entity->getHealth();
			const ICombatant* combatant = dynamic_cast<const ICombatant*>(entity);
			int damage = combatant ? combatant->getDamage() : 0;
			entities.emplace_back(EntityType::ENEMY, index, health.first, health.second, damage);
		}
	}
	
	auto ally_indices = game_state.getEntityManager().findEntitiesByType(EntityType::ALLY);
	for (int index : ally_indices) {
		const IEntity* entity = game_state.getEntityManager().getEntity(index);
		if (entity && entity->alive()) {
			auto health = entity->getHealth();
			const ICombatant* combatant = dynamic_cast<const ICombatant*>(entity);
			int damage = combatant ? combatant->getDamage() : 0;
			entities.emplace_back(EntityType::ALLY, index, health.first, health.second, damage);
		}
	}
	
	auto barracks_indices = game_state.getEntityManager().findEntitiesByType(EntityType::BARRACKS);
	for (int index : barracks_indices) {
		const IEntity* entity = game_state.getEntityManager().getEntity(index);
		if (entity && entity->alive()) {
			auto health = entity->getHealth();
			const ICombatant* combatant = dynamic_cast<const ICombatant*>(entity);
			int damage = combatant ? combatant->getDamage() : 0;
			entities.emplace_back(EntityType::BARRACKS, index, health.first, health.second, damage);
		}
	}
	
	auto tower_indices = game_state.getEntityManager().findEntitiesByType(EntityType::TOWER);
	for (int index : tower_indices) {
		const IEntity* entity = game_state.getEntityManager().getEntity(index);
		if (entity && entity->alive()) {
			auto health = entity->getHealth();
			const ICombatant* combatant = dynamic_cast<const ICombatant*>(entity);
			int damage = combatant ? combatant->getDamage() : 0;
			entities.emplace_back(EntityType::TOWER, index, health.first, health.second, damage);
		}
	}
	
	return entities;
}

std::vector<int> GamePersistence::collectFieldCells() const {
	std::vector<int> field_cells;
	int field_size = game_state.getField().getWidth() * game_state.getField().getHeight();
	field_cells.reserve(field_size);
	
	for (int i = 0; i < field_size; ++i) {
		field_cells.push_back(static_cast<int>(game_state.getField().getCell(i).getType()));
	}
	
	return field_cells;
}

void GamePersistence::saveGame() {
	const Player* player = getPlayer();
	if (!player) {
		throw GameException("Невозможно сохранить: игрок не найден", "GamePersistence::saveGame");
	}
	
	std::vector<EntityData> entities = collectAllEntities();
	std::vector<int> field_cells = collectFieldCells();
	
	state_manager.saveGame(
		level_director.getCurrentLevel(),
		game_state.getGameTurn(),
		score_system.getScore(),
		score_system.getThreshold(),
		game_state.getField().getWidth(),
		game_state.getField().getHeight(),
		game_state.getPlayerIndex(),
		player->getHealth().first,
		player->getHealth().second,
		player->getInt(),
		player->getDex(),
		player->getStr(),
		player->getSpellHand().getMaxSize(),
		player->getSpellHand().getCurrentSize(),
		player->melee(),
		player->getResources().getPoints(),
		player->getResources().getKills(),
		field_cells,
		entities
	);
}

void GamePersistence::restoreFieldFromSave(const std::vector<int>& field_cells, int width, int height) {
	game_state.getField() = GameField::createEmpty(width, height);
	
	for (size_t i = 0; i < field_cells.size(); ++i) {
		game_state.getField().setCellType(i, static_cast<CellType>(field_cells[i]));
	}
}

void GamePersistence::restorePlayerFromSave(const GameStateManager::LoadedGameState& state) {
	auto player = std::make_unique<Player>(
		state.player_intel, state.player_dex, state.player_str, 5,
		state.player_max_hp,
		state.player_is_melee ? 3 : 0,
		state.player_is_melee ? 0 : 2,
		state.player_hand_size
	);
	
	int hp_diff = state.player_max_hp - state.player_current_hp;
	if (hp_diff > 0) {
		player->causeDamage(hp_diff);
	}
	
	if (!state.player_is_melee) {
		player->setWeapon();
	}
	
	for (size_t i = 0; i < state.player_spell_count; ++i) {
		player->getSpellHand().addSpell(game_systems.getSpellFactory().createRandomSpell());
	}
	
	player->getResources().setPoints(state.player_points);
	player->getResources().setKills(state.player_kills);
	
	game_state.getEntityManager().addEntity(std::move(player), game_state.getPlayerIndex());
}

void GamePersistence::restoreEntitiesFromSave(const std::vector<EntityData>& entities) {
	for (const auto& entity_data : entities) {
		auto entity = game_systems.getEntityFactory().createEntity(
			entity_data.type,
			entity_data.damage,
			entity_data.max_hp,
			5
		);
		
		if (entity) {
			int hp_diff = entity_data.max_hp - entity_data.current_hp;
			if (hp_diff > 0) {
				entity->causeDamage(hp_diff);
			}
			
			game_state.getEntityManager().addEntity(std::move(entity), entity_data.index);
		}
	}
}

void GamePersistence::loadGame() {
	auto state = state_manager.loadGame();
	
	game_state.getEntityManager() = EntityManager();
	game_state.setPlayerIndex(-1);
	game_state.setGameTurn(0);
	
	level_director.setLevel(state.level);
	game_state.setGameTurn(state.turn);
	score_system = ScoreSystem(state.score_threshold);
	score_system.addScore(state.score);
	
	restoreFieldFromSave(state.field_cells, state.field_width, state.field_height);
	game_state.setPlayerIndex(state.player_index);
	
	restorePlayerFromSave(state);
	
	restoreEntitiesFromSave(state.entities);
}

bool GamePersistence::hasSaveFile() const {
	return state_manager.saveFileExists();
}
