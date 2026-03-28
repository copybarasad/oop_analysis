#include "GamePersistence.h"
#include <filesystem>
#include <fstream>
#include "GameSaveException.h"
#include "GameLoadException.h"

int GamePersistence::serializeAttackType(AttackType type) {
	return (type == AttackType::MELEE) ? 0 : 1;
}

AttackType GamePersistence::deserializeAttackType(int value) {
	if (value == 0) {
		return AttackType::MELEE;
	}
	if (value == 1) {
		return AttackType::RANGED;
	}
	throw GameLoadException("Файл сохранения поврежден: неизвестный тип атаки");
}

int GamePersistence::serializeCellType(CellType type) {
	return static_cast<int>(type);
}

CellType GamePersistence::deserializeCellType(int value) {
	if (value == 0) {
		return CellType::PASSABLE;
	}
	if (value == 1) {
		return CellType::IMPASSABLE;
	}
	if (value == 2) {
		return CellType::SLOWING;
	}
	if (value == 3) {
		return CellType::TRAP;
	}
	throw GameLoadException("Файл сохранения поврежден: неизвестный тип клетки");
}

int GamePersistence::serializeSpellId(SpellId id) {
	return static_cast<int>(id);
}

SpellId GamePersistence::deserializeSpellId(int value) {
	if (value < static_cast<int>(SpellId::DIRECT_DAMAGE) ||
		value > static_cast<int>(SpellId::ENHANCEMENT)) {
		throw GameLoadException("Файл сохранения поврежден: неизвестное заклинание");
	}
	return static_cast<SpellId>(value);
}

void GamePersistence::ensureDirectoryExists(const std::string& filePath) const {
	const std::filesystem::path savePath(filePath);
	const auto parent = savePath.parent_path();

	if (!parent.empty() && !std::filesystem::exists(parent)) {
		std::filesystem::create_directories(parent);
	}
}

void GamePersistence::save(const GameSnapshot& snapshot, const std::string& filePath) const {
	ensureDirectoryExists(filePath);

	std::ofstream out(filePath, std::ios::trunc);
	if (!out) {
		throw GameSaveException("Не удалось открыть файл для сохранения: " + filePath);
	}

	out << snapshot.level << '\n';

	const auto& field = snapshot.field;
	out << field.width << ' ' << field.height << '\n';
	out << field.playerPosition.x << ' ' << field.playerPosition.y << '\n';

	const PlayerState& playerState = field.playerState;
	out << playerState.health << ' ' << playerState.maxHealth << ' '
		<< playerState.meleeDamage << ' ' << playerState.rangedDamage << ' '
		<< playerState.score << ' ' << serializeAttackType(playerState.attackType) << ' '
		<< (playerState.slowed ? 1 : 0) << ' ' << playerState.enemiesDefeated << ' '
		<< playerState.enhancementStack << ' ' << playerState.spellHandCapacity << '\n';

	out << playerState.spellIds.size() << '\n';
	for (SpellId id : playerState.spellIds) {
		out << serializeSpellId(id) << ' ';
	}
	out << '\n';

	for (const auto& row : field.cellTypes) {
		for (CellType type : row) {
			out << serializeCellType(type) << ' ';
		}
		out << '\n';
	}

	out << field.enemies.size() << '\n';
	for (const auto& enemy : field.enemies) {
		out << enemy.position.x << ' ' << enemy.position.y << ' '
			<< enemy.health << ' ' << enemy.damage << '\n';
	}

	out << field.buildings.size() << '\n';
	for (const auto& building : field.buildings) {
		out << building.position.x << ' ' << building.position.y << ' '
			<< building.turnsToSpawn << ' ' << building.currentTurn << ' '
			<< building.spawnHealth << ' ' << building.spawnDamage << '\n';
	}

	out << field.towers.size() << '\n';
	for (const auto& tower : field.towers) {
		out << tower.position.x << ' ' << tower.position.y << ' '
			<< tower.range << ' ' << tower.damage << ' '
			<< (tower.canAttack ? 1 : 0) << ' ' << tower.cooldown << ' '
			<< tower.currentCooldown << '\n';
	}

	out << field.allies.size() << '\n';
	for (const auto& ally : field.allies) {
		out << ally.position.x << ' ' << ally.position.y << ' '
			<< ally.health << ' ' << ally.damage << ' ' << ally.lifetime << '\n';
	}

	out << field.traps.size() << '\n';
	for (const auto& trap : field.traps) {
		out << trap.position.x << ' ' << trap.position.y << ' ' << trap.damage << '\n';
	}

	if (!out.good()) {
		throw GameSaveException("Ошибка записи файла сохранения: " + filePath);
	}
}

GameSnapshot GamePersistence::load(const std::string& filePath) const {
	std::ifstream in(filePath);
	if (!in) {
		throw GameLoadException("Не удалось открыть файл сохранения: " + filePath);
	}

	GameSnapshot snapshot{};
	in >> snapshot.level;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: уровень не прочитан");
	}

	GameFieldSnapshot fieldSnapshot{};
	in >> fieldSnapshot.width >> fieldSnapshot.height;
	in >> fieldSnapshot.playerPosition.x >> fieldSnapshot.playerPosition.y;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: параметры поля");
	}

	PlayerState playerState{};
	int attackTypeValue = 0;
	int slowedValue = 0;
	in >> playerState.health >> playerState.maxHealth
		>> playerState.meleeDamage >> playerState.rangedDamage
		>> playerState.score >> attackTypeValue >> slowedValue
		>> playerState.enemiesDefeated >> playerState.enhancementStack
		>> playerState.spellHandCapacity;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: параметры игрока");
	}
	playerState.attackType = deserializeAttackType(attackTypeValue);
	playerState.slowed = slowedValue != 0;

	size_t spellCount = 0;
	in >> spellCount;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: количество заклинаний");
	}
	playerState.spellIds.clear();
	for (size_t i = 0; i < spellCount; ++i) {
		int spellValue = 0;
		in >> spellValue;
		if (!in) {
			throw GameLoadException("Файл сохранения поврежден: список заклинаний");
		}
		playerState.spellIds.push_back(deserializeSpellId(spellValue));
	}
	fieldSnapshot.playerState = playerState;

	fieldSnapshot.cellTypes.assign(fieldSnapshot.height, std::vector<CellType>(fieldSnapshot.width, CellType::PASSABLE));
	for (int y = 0; y < fieldSnapshot.height; ++y) {
		for (int x = 0; x < fieldSnapshot.width; ++x) {
			int cellValue = 0;
			in >> cellValue;
			if (!in) {
				throw GameLoadException("Файл сохранения поврежден: данные клетки поля");
			}
			fieldSnapshot.cellTypes[y][x] = deserializeCellType(cellValue);
		}
	}

	size_t enemyCount = 0;
	in >> enemyCount;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: количество врагов");
	}
	for (size_t i = 0; i < enemyCount; ++i) {
		EnemySnapshot enemySnapshot{};
		in >> enemySnapshot.position.x >> enemySnapshot.position.y
			>> enemySnapshot.health >> enemySnapshot.damage;
		if (!in) {
			throw GameLoadException("Файл сохранения поврежден: данные врага");
		}
		fieldSnapshot.enemies.push_back(enemySnapshot);
	}

	size_t buildingCount = 0;
	in >> buildingCount;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: количество зданий");
	}
	for (size_t i = 0; i < buildingCount; ++i) {
		BuildingSnapshot buildingSnapshot{};
		in >> buildingSnapshot.position.x >> buildingSnapshot.position.y
			>> buildingSnapshot.turnsToSpawn >> buildingSnapshot.currentTurn
			>> buildingSnapshot.spawnHealth >> buildingSnapshot.spawnDamage;
		if (!in) {
			throw GameLoadException("Файл сохранения поврежден: данные здания");
		}
		fieldSnapshot.buildings.push_back(buildingSnapshot);
	}

	size_t towerCount = 0;
	in >> towerCount;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: количество башен");
	}
	for (size_t i = 0; i < towerCount; ++i) {
		TowerSnapshot towerSnapshot{};
		int canAttackValue = 0;
		in >> towerSnapshot.position.x >> towerSnapshot.position.y
			>> towerSnapshot.range >> towerSnapshot.damage
			>> canAttackValue >> towerSnapshot.cooldown >> towerSnapshot.currentCooldown;
		if (!in) {
			throw GameLoadException("Файл сохранения поврежден: данные башни");
		}
		towerSnapshot.canAttack = (canAttackValue != 0);
		fieldSnapshot.towers.push_back(towerSnapshot);
	}

	size_t allyCount = 0;
	in >> allyCount;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: количество союзников");
	}
	for (size_t i = 0; i < allyCount; ++i) {
		AllySnapshot allySnapshot{};
		in >> allySnapshot.position.x >> allySnapshot.position.y
			>> allySnapshot.health >> allySnapshot.damage >> allySnapshot.lifetime;
		if (!in) {
			throw GameLoadException("Файл сохранения поврежден: данные союзника");
		}
		fieldSnapshot.allies.push_back(allySnapshot);
	}

	size_t trapCount = 0;
	in >> trapCount;
	if (!in) {
		throw GameLoadException("Файл сохранения поврежден: количество ловушек");
	}
	for (size_t i = 0; i < trapCount; ++i) {
		TrapSnapshot trapSnapshot{};
		in >> trapSnapshot.position.x >> trapSnapshot.position.y >> trapSnapshot.damage;
		if (!in) {
			throw GameLoadException("Файл сохранения поврежден: данные ловушки");
		}
		fieldSnapshot.traps.push_back(trapSnapshot);
	}

	snapshot.field = fieldSnapshot;
	return snapshot;
}

bool GamePersistence::hasSave(const std::string& filePath) const {
	return std::filesystem::exists(filePath);
}

