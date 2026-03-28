#include "GameSerializer.hpp"
#include "DirectDamageSpell.hpp"
#include "AreaDamageSpell.hpp"
#include "TrapSpell.hpp"
#include "GameException.hpp"
#include <stdexcept>

void GameSerializer::serializeGame(SaveManager& save, 
									const GameStateManager& stateManager,
									const Player& player,
									const Field& field,
									const WorldManager& world) {
	int version = 1;
	save << version;

	int stateInt = static_cast<int>(stateManager.getState());
	save << stateInt;
	save << stateManager.getCurrentLevel();
	save << stateManager.getKillsRequired();
	save << stateManager.getKillsForReward();
	
	save << player.getHP();
	save << player.getMaxHP();
	int modeInt = static_cast<int>(player.getMode());
	save << modeInt;
	
	save << field.getWidth();
	save << field.getHeight();
	
	for (int x = 0; x < field.getWidth(); ++x) {
		for (int y = 0; y < field.getHeight(); ++y) {
			Coords pos{x, y};
			const Cell& cell = field.cellAt(pos);
			int walkable = cell.isWalkable() ? 1 : 0;
			int typeInt = static_cast<int>(cell.getType());
			int occInt = static_cast<int>(cell.getOccupant());
			int occId = cell.getOccupantId();
			save << walkable << typeInt << occInt << occId;
		}
	}
	
	Coords playerPos = world.getPlayerPos();
	save << playerPos.x << playerPos.y;
	save << world.getKillCount();
	save << (world.isPlayerSlowed() ? 1 : 0);
	
	const auto& enemies = world.getEnemies();
	save << static_cast<int>(enemies.size());
	for (const auto& entry : enemies) {
		save << entry.enemy.getHP();
		save << entry.enemy.getDamage();
		save << entry.pos.x << entry.pos.y;
	}
	
	const auto& buildings = world.getBuildings();
	save << static_cast<int>(buildings.size());
	for (const auto& entry : buildings) {
		save << entry.building.getHP();
		save << entry.building.getSpawnInterval();
		save << entry.building.getTurnsUntilSpawn();
		save << entry.building.getSpawnCount();
		save << entry.building.getMaxSpawnCount();
		save << entry.pos.x << entry.pos.y;
	}
	
	const auto& towers = world.getTowers();
	save << static_cast<int>(towers.size());
	for (const auto& entry : towers) {
		save << entry.tower.getHP();
		save << entry.tower.getDamage();
		save << entry.tower.getRange();
		save << entry.tower.getCooldownRemaining();
		save << entry.pos.x << entry.pos.y;
	}
	
	const auto& traps = world.getSpellManager().getTraps();
	int activeTrapCount = 0;
	for (const auto& trap : traps) {
		if (trap.isActive()) {
			activeTrapCount++;
		}
	}
	save << activeTrapCount;
	for (const auto& trap : traps) {
		if (trap.isActive()) {
			save << trap.getPosition().x << trap.getPosition().y;
			save << trap.getDamage();
		}
	}
	
	const auto& hand = world.getSpellManager().getHand();
	save << static_cast<int>(hand.size());
	for (const auto& spell : hand) {
		if (!spell) continue;
		std::string name = spell->getName();
		int spellType = 0;
		if (name.find("Область") != std::string::npos) {
			spellType = 1;
		} else if (name.find("Ловушка") != std::string::npos) {
			spellType = 2;
		}
		save << spellType;
		save << spell->getDamage();
		save << spell->getRange();
	}
}

void GameSerializer::deserializeGame(SaveManager& load,
									  GameStateManager& stateManager,
									  Player& player,
									  std::unique_ptr<Field>& field,
									  std::unique_ptr<WorldManager>& world) {
	int version;
	load >> version;
	if (version != 1) {
		throw DataException("Неподдерживаемая версия формата сохранения: " + std::to_string(version), "GameSerializer::deserializeGame");
	}
	
	int stateInt;
	load >> stateInt;
	stateManager.setState(static_cast<GameState>(stateInt));
	int currentLevel, killsRequired, killsForReward;
	load >> currentLevel >> killsRequired >> killsForReward;
	stateManager.setCurrentLevel(currentLevel);
	stateManager.setKillsRequired(killsRequired);
	stateManager.setKillsForReward(killsForReward);
	
	int hp, maxHP, modeInt;
	load >> hp >> maxHP >> modeInt;
	player = Player(maxHP, 15);
	player.setHP(hp);
	if (static_cast<AttackMode>(modeInt) != player.getMode()) {
		player.toggleMode();
	}
	
	int width, height;
	load >> width >> height;
	field = std::make_unique<Field>(width, height);
	
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			Coords pos{x, y};
			Cell& cell = field->cellAt(pos);
			int walkable, typeInt, occInt, occId;
			load >> walkable >> typeInt >> occInt >> occId;
			cell.setWalkable(walkable != 0);
			cell.setType(static_cast<CellType>(typeInt));
			cell.clearOccupant();
		}
	}
	
	world = std::make_unique<WorldManager>(*field);
	
	int playerX, playerY, playerSlowed;
	load >> playerX >> playerY;
	world->addPlayer(Coords{playerX, playerY});
	
	int killCount;
	load >> killCount;
	world->setKillCount(killCount);
	
	load >> playerSlowed;
	world->setPlayerSlowed(playerSlowed != 0);
	
	int enemyCount;
	load >> enemyCount;
	for (int i = 0; i < enemyCount; ++i) {
		int hp, damage, x, y;
		load >> hp >> damage >> x >> y;
		Enemy enemy(hp, damage);
		world->addEnemy(Coords{x, y}, enemy);
	}
	
	int buildingCount;
	load >> buildingCount;
	for (int i = 0; i < buildingCount; ++i) {
		int hp, spawnInterval, turnsUntilSpawn, spawnCount, maxSpawnCount, x, y;
		load >> hp >> spawnInterval >> turnsUntilSpawn >> spawnCount >> maxSpawnCount >> x >> y;
		EnemyBuilding building(hp, spawnInterval, maxSpawnCount);
		building.setHP(hp);
		building.setTurnsUntilSpawn(turnsUntilSpawn);
		building.setSpawnCount(spawnCount);
		world->addBuilding(Coords{x, y}, building);
	}
	
	int towerCount;
	load >> towerCount;
	for (int i = 0; i < towerCount; ++i) {
		int hp, damage, range, cooldownRemaining, x, y;
		load >> hp >> damage >> range >> cooldownRemaining >> x >> y;
		int baseCooldown = 2;
		EnemyTower tower(hp, damage, range, baseCooldown);
		tower.setCooldownRemaining(cooldownRemaining);
		world->addTower(Coords{x, y}, tower);
	}
	
	int trapCount;
	load >> trapCount;
	for (int i = 0; i < trapCount; ++i) {
		int x, y, damage;
		load >> x >> y >> damage;
		world->getSpellManager().placeTrap(Coords{x, y}, damage);
	}
	
	int spellCount;
	load >> spellCount;
	world->getSpellManager().clearHand();
	for (int i = 0; i < spellCount; ++i) {
		int spellType, damage, range;
		load >> spellType >> damage >> range;
		std::unique_ptr<ISpell> spell;
		if (spellType == 0) {
			spell = std::make_unique<DirectDamageSpell>(damage, range);
		} else if (spellType == 1) {
			spell = std::make_unique<AreaDamageSpell>(damage, range);
		} else if (spellType == 2) {
			spell = std::make_unique<TrapSpell>(damage, range);
		}
		if (spell) {
			world->getSpellManager().addSpellForLoad(std::move(spell));
		}
	}
}

