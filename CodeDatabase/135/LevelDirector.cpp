#include "LevelDirector.h"
#include "RandomGenerator.h"
#include "SpellFactory.h"
#include "Enemy.h"
#include "EnemyBarracks.h"
#include "GameConstants.h"

LevelDirector::LevelDirector(RandomGenerator& random_gen, SpellFactory& spell_fact, EntityFactory& entity_fact)
	: level_manager(), rng(random_gen), spell_factory(spell_fact), entity_factory(entity_fact) {}

void LevelDirector::reset() {
	level_manager = LevelManager();
}

void LevelDirector::spawnEnemies(GameField& field, EntityManager& entity_manager, const LevelConfig& config) {
	int enemy_count = rng.getInt(config.min_enemies, config.max_enemies);
	for (int i = 0; i < enemy_count; ++i) {
		int x, y, index;
		do {
			x = rng.getInt(0, field.getWidth() - 1);
			y = rng.getInt(0, field.getHeight() - 1);
			index = field.coordToIndex(x, y);
		} while (!field.getCell(index).isPassable() || entity_manager.hasEntity(index));
		
		auto enemy = entity_factory.createEntity(EntityType::ENEMY, 
		                                          config.enemy_base_damage, 
		                                          config.enemy_base_health);
		entity_manager.addEntity(std::move(enemy), index);
	}
}

void LevelDirector::spawnBarracks(GameField& field, EntityManager& entity_manager, const LevelConfig& config) {
	int barracks_count = rng.getInt(config.min_barracks, config.max_barracks);
	for (int i = 0; i < barracks_count; ++i) {
		int x, y, index;
		do {
			x = rng.getInt(0, field.getWidth() - 1);
			y = rng.getInt(0, field.getHeight() - 1);
			index = field.coordToIndex(x, y);
		} while (!field.getCell(index).isPassable() || entity_manager.hasEntity(index));
		
		auto barracks = entity_factory.createEntity(EntityType::BARRACKS, 
		                                             0, 
		                                             config.barracks_health, 
		                                             config.barracks_spawn_cooldown);
		entity_manager.addEntity(std::move(barracks), index);
	}
}

LevelDirector::LevelInitResult LevelDirector::initializeLevel(int level) {
	LevelConfig config(level);
	
	GameField field(config.field_width, config.field_height, rng);
	EntityManager entity_manager;
	
	int player_start_x = 1;
	int player_start_y = 1;
	int player_index = field.coordToIndex(player_start_x, player_start_y);
	
	auto player = createPlayer();
	entity_manager.addEntity(std::move(player), player_index);
	
	spawnEnemies(field, entity_manager, config);
	spawnBarracks(field, entity_manager, config);
	
	return LevelInitResult{std::move(field), std::move(entity_manager), player_index, config.score_threshold};
}

std::unique_ptr<Player> LevelDirector::createPlayer() {
	auto player = std::make_unique<Player>(10, 10, 10, 5, 100, 3, 2, 5);
	player->getSpellHand().addSpell(spell_factory.createRandomSpell());
	return player;
}

void LevelDirector::advanceLevel() {
	level_manager.advanceLevel();
}

int LevelDirector::getCurrentLevel() const {
	return level_manager.getCurrentLevel();
}

void LevelDirector::setLevel(int level) {
	level_manager.setLevel(level);
}

void LevelDirector::preparePlayerForNextLevel(Player& player) {
	player.restoreHealth();
	player.getSpellHand().removeHalfSpells();
}
