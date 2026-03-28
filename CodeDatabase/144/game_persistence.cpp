#include "game_persistence.h"
#include "file_modification_exception.h"
#include <fstream>
#include <sstream>
#include <cstdint>
#include <algorithm>

namespace {
constexpr const char* SAVE_SIGNATURE = "SAVE_V1";
}

int GamePersistence::convertBoolToInt(bool value) const {
	return value ? 1 : 0;
}

bool GamePersistence::convertIntToBool(int value) const {
	return value != 0;
}

int GamePersistence::convertCombatModeToInt(CombatMode mode) const {
	return (mode == CombatMode::RANGED) ? 1 : 0;
}

CombatMode GamePersistence::convertIntToCombatMode(int value) const {
	return (value == 1) ? CombatMode::RANGED : CombatMode::MELEE;
}

uint64_t GamePersistence::calculateChecksum(const std::string& filePath) const {
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		throw FileAccessException(filePath, "open for checksum calculation");
	}
	
	file.seekg(0, std::ios::end);
	std::streampos fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	
	return calculateChecksumFromStream(file, 0, fileSize);
}

uint64_t GamePersistence::calculateChecksumFromStream(std::ifstream& stream, 
		std::streampos startPos, std::streampos endPos) const {
	std::streampos originalPos = stream.tellg();
	stream.seekg(startPos, std::ios::beg);
	
	uint64_t checksum = 0;
	constexpr size_t bufferSize = 4096;
	char buffer[bufferSize];
	
	std::streampos currentPos = startPos;
	while (currentPos < endPos) {
		std::streampos remaining = endPos - currentPos;
		size_t readSize = static_cast<size_t>(std::min(remaining, 
			static_cast<std::streampos>(bufferSize)));
		
		stream.read(buffer, readSize);
		size_t actualRead = static_cast<size_t>(stream.gcount());
		
		for (size_t i = 0; i < actualRead; ++i) {
			checksum += static_cast<unsigned char>(buffer[i]);
			checksum = (checksum << 1) | (checksum >> 63);
		}
		
		currentPos += actualRead;
		if (actualRead < readSize) {
			break;
		}
	}
	
	stream.seekg(originalPos, std::ios::beg);
	return checksum;
}

void GamePersistence::writeChecksumToStream(std::ofstream& stream, uint64_t checksum) const {
	if (!stream.good()) {
		throw std::runtime_error("Stream is not in good state for writing checksum");
	}
	stream.write(reinterpret_cast<const char*>(&checksum), sizeof(checksum));
	if (!stream.good()) {
		throw std::runtime_error("Failed to write checksum to stream");
	}
}

uint64_t GamePersistence::readChecksumFromStream(std::ifstream& stream) const {
	if (!stream.good()) {
		throw CorruptedDataException("stream state before reading checksum");
	}
	
	uint64_t checksum = 0;
	stream.read(reinterpret_cast<char*>(&checksum), sizeof(checksum));
	
	if (stream.fail() || stream.gcount() != sizeof(checksum)) {
		throw CorruptedDataException("checksum data (failed to read checksum value)");
	}
	
	return checksum;
}

void GamePersistence::writeSpellStates(std::ostream& out, const std::vector<SpellState>& spells) const {
	out << spells.size() << '\n';
	for (const auto& state : spells) {
		out << static_cast<int>(state.type) << ' ' << state.enhancementLevel << '\n';
	}
}

std::vector<SpellState> GamePersistence::readSpellStates(std::istream& in) const {
	size_t count = 0;
	in >> count;
	if (in.fail() || count > 1000) {
		throw CorruptedDataException("spell count (value: " + std::to_string(count) + ")");
	}
	std::vector<SpellState> spells;
	spells.reserve(count);
	for (size_t i = 0; i < count; ++i) {
		int type = 0;
		int enhancement = 0;
		in >> type >> enhancement;
		if (in.fail()) {
			throw CorruptedDataException("spell data at index " + std::to_string(i));
		}
		if (type < 0 || type > 10) {
			throw InvalidDataException("spell type at index " + std::to_string(i), 
				"invalid type value: " + std::to_string(type));
		}
		if (enhancement < 0 || enhancement > 100) {
			throw InvalidDataException("spell enhancement at index " + std::to_string(i), 
				"invalid enhancement value: " + std::to_string(enhancement));
		}
		spells.push_back(SpellState{static_cast<SpellType>(type), enhancement});
	}
	return spells;
}

template <typename T, typename Writer>
void GamePersistence::writeCollection(std::ostream& out, const std::vector<T>& collection, Writer writer) const {
	out << collection.size() << '\n';
	for (const auto& item : collection) {
		writer(out, item);
	}
}

template <typename T, typename Reader>
std::vector<T> GamePersistence::readCollection(std::istream& in, Reader reader) const {
	size_t count = 0;
	in >> count;
	std::vector<T> collection;
	collection.reserve(count);
	for (size_t i = 0; i < count; ++i) {
		T item = reader(in);
		collection.push_back(item);
	}
	return collection;
}

void GamePersistence::writeField(std::ostream& out, const Field::Snapshot& field) const {
	out << field.width << ' ' << field.height << '\n';
	for (const auto& cell : field.cells) {
		out << static_cast<int>(cell) << ' ';
	}
	out << '\n';
}

Field::Snapshot GamePersistence::readField(std::istream& in) const {
	Field::Snapshot field;
	in >> field.width >> field.height;
	if (in.fail()) {
		throw CorruptedDataException("field dimensions");
	}
	if (field.width < 10 || field.width > 30 || field.height < 10 || field.height > 30) {
		throw InvalidDataException("field dimensions", 
			"width=" + std::to_string(field.width) + " height=" + std::to_string(field.height));
	}
	field.cells.resize(field.width * field.height);
	for (int i = 0; i < field.width * field.height; ++i) {
		int cellType = 0;
		in >> cellType;
		if (in.fail()) {
			throw CorruptedDataException("field cell at index " + std::to_string(i));
		}
		if (cellType < 0 || cellType > 2) {
			throw InvalidDataException("field cell type at index " + std::to_string(i), 
				"invalid cell type: " + std::to_string(cellType));
		}
		field.cells[i] = static_cast<CellType>(cellType);
	}
	return field;
}

void GamePersistence::writePlayer(std::ostream& out, const Player::Snapshot& player) const {
	out << player.maximumHealth << ' ' << player.currentHealth << ' '
	    << player.meleeAttackDamage << ' ' << player.rangedAttackDamage << ' '
	    << player.score << ' ' << player.rangedAttackRange << ' '
	    << player.positionX << ' ' << player.positionY << ' '
	    << player.currentMana << ' ' << player.maximumMana << ' '
	    << player.currentEnhancementLevel << ' '
	    << convertCombatModeToInt(player.combatMode) << ' '
	    << convertBoolToInt(player.slowed) << '\n';
	writeSpellStates(out, player.handSpells);
}

Player::Snapshot GamePersistence::readPlayer(std::istream& in) const {
	Player::Snapshot player;
	int combatModeValue = 0;
	int slowed = 0;
	in >> player.maximumHealth >> player.currentHealth
	   >> player.meleeAttackDamage >> player.rangedAttackDamage
	   >> player.score >> player.rangedAttackRange
	   >> player.positionX >> player.positionY
	   >> player.currentMana >> player.maximumMana
	   >> player.currentEnhancementLevel
	   >> combatModeValue >> slowed;
	if (in.fail()) {
		throw CorruptedDataException("player data");
	}
	player.combatMode = convertIntToCombatMode(combatModeValue);
	player.slowed = convertIntToBool(slowed);
	player.handSpells = readSpellStates(in);
	return player;
}

void GamePersistence::writeEnemy(std::ostream& out, const Enemy::Snapshot& enemy) const {
	out << enemy.health << ' ' << enemy.maxHealth << ' '
	    << enemy.damage << ' ' << enemy.positionX << ' '
	    << enemy.positionY << '\n';
}

Enemy::Snapshot GamePersistence::readEnemy(std::istream& in) const {
	Enemy::Snapshot enemy;
	in >> enemy.health >> enemy.maxHealth >> enemy.damage
	   >> enemy.positionX >> enemy.positionY;
	if (in.fail()) {
		throw CorruptedDataException("enemy data");
	}
	if (enemy.maxHealth <= 0 || enemy.health < 0 || enemy.health > enemy.maxHealth) {
		throw InvalidDataException("enemy health", 
			"max=" + std::to_string(enemy.maxHealth) + " current=" + std::to_string(enemy.health));
	}
	if (enemy.damage < 0 || enemy.damage > 1000) {
		throw InvalidDataException("enemy damage", "value: " + std::to_string(enemy.damage));
	}
	return enemy;
}

void GamePersistence::writeBuilding(std::ostream& out, const EnemyBuilding::Snapshot& building) const {
	out << building.spawnInterval << ' ' << building.turnsUntilSpawn << ' '
	    << building.positionX << ' ' << building.positionY << ' '
	    << building.health << ' ' << building.maxHealth << '\n';
}

EnemyBuilding::Snapshot GamePersistence::readBuilding(std::istream& in) const {
	EnemyBuilding::Snapshot building;
	in >> building.spawnInterval >> building.turnsUntilSpawn
	   >> building.positionX >> building.positionY
	   >> building.health >> building.maxHealth;
	if (in.fail()) {
		throw CorruptedDataException("building data");
	}
	if (building.maxHealth <= 0 || building.health < 0 || building.health > building.maxHealth) {
		throw InvalidDataException("building health", 
			"max=" + std::to_string(building.maxHealth) + " current=" + std::to_string(building.health));
	}
	if (building.spawnInterval < 0 || building.turnsUntilSpawn < 0) {
		throw InvalidDataException("building spawn timing", 
			"interval=" + std::to_string(building.spawnInterval) + 
			" turnsUntil=" + std::to_string(building.turnsUntilSpawn));
	}
	return building;
}

void GamePersistence::writeTower(std::ostream& out, const EnemyTower::Snapshot& tower) const {
	writeBuilding(out, tower.buildingSnapshot);
	out << tower.attackRange << ' ' << tower.damage << ' '
	    << convertBoolToInt(tower.canAttack) << ' '
	    << tower.attackCooldown << ' ' << tower.currentCooldown << ' '
	    << convertBoolToInt(tower.readyToAttack) << '\n';
}

EnemyTower::Snapshot GamePersistence::readTower(std::istream& in) const {
	EnemyTower::Snapshot tower;
	tower.buildingSnapshot = readBuilding(in);
	int canAttack = 0;
	int ready = 0;
	in >> tower.attackRange >> tower.damage >> canAttack
	   >> tower.attackCooldown >> tower.currentCooldown >> ready;
	if (in.fail()) {
		throw CorruptedDataException("tower data");
	}
		tower.canAttack = convertIntToBool(canAttack);
		tower.readyToAttack = convertIntToBool(ready);
	return tower;
}

void GamePersistence::writeTrap(std::ostream& out, const Trap::Snapshot& trap) const {
	out << trap.positionX << ' ' << trap.positionY << ' '
	    << trap.damage << ' ' << convertBoolToInt(trap.active) << '\n';
}

Trap::Snapshot GamePersistence::readTrap(std::istream& in) const {
	Trap::Snapshot trap;
	int active = 0;
	in >> trap.positionX >> trap.positionY >> trap.damage >> active;
	if (in.fail()) {
		throw CorruptedDataException("trap data");
	}
	if (trap.damage < 0 || trap.damage > 1000) {
		throw InvalidDataException("trap damage", "value: " + std::to_string(trap.damage));
	}
	trap.active = convertIntToBool(active);
	return trap;
}

void GamePersistence::writeAlly(std::ostream& out, const Ally::Snapshot& ally) const {
	out << ally.health << ' ' << ally.damage << ' '
	    << ally.positionX << ' ' << ally.positionY << ' '
	    << ally.movementCooldown << ' ' << convertBoolToInt(ally.alive) << '\n';
}

Ally::Snapshot GamePersistence::readAlly(std::istream& in) const {
	Ally::Snapshot ally;
	int alive = 0;
	in >> ally.health >> ally.damage >> ally.positionX >> ally.positionY
	   >> ally.movementCooldown >> alive;
	if (in.fail()) {
		throw CorruptedDataException("ally data");
	}
	if (ally.health < 0 || ally.health > 10000) {
		throw InvalidDataException("ally health", "value: " + std::to_string(ally.health));
	}
	if (ally.damage < 0 || ally.damage > 1000) {
		throw InvalidDataException("ally damage", "value: " + std::to_string(ally.damage));
	}
	if (ally.movementCooldown < 0 || ally.movementCooldown > 100) {
		throw InvalidDataException("ally movement cooldown", 
			"value: " + std::to_string(ally.movementCooldown));
	}
	ally.alive = convertIntToBool(alive);
	return ally;
}

void GamePersistence::writeSpellEnhancements(std::ostream& out, const std::map<SpellType, int>& enhancements) const {
	out << enhancements.size() << '\n';
	for (const auto& pair : enhancements) {
		out << static_cast<int>(pair.first) << ' ' << pair.second << '\n';
	}
}

std::map<SpellType, int> GamePersistence::readSpellEnhancements(std::istream& in) const {
	std::map<SpellType, int> enhancements;
	size_t count = 0;
	in >> count;
	if (in.fail() || count > 100) {
		throw CorruptedDataException("spell enhancements count (value: " + std::to_string(count) + ")");
	}
	for (size_t i = 0; i < count; ++i) {
		int type = 0;
		int level = 0;
		in >> type >> level;
		if (in.fail()) {
			throw CorruptedDataException("spell enhancement data at index " + std::to_string(i));
		}
		if (type < 0 || type > 10) {
			throw InvalidDataException("spell enhancement type at index " + std::to_string(i), 
				"invalid type value: " + std::to_string(type));
		}
		if (level < 0 || level > 100) {
			throw InvalidDataException("spell enhancement level at index " + std::to_string(i), 
				"invalid level value: " + std::to_string(level));
		}
		enhancements[static_cast<SpellType>(type)] = level;
	}
	return enhancements;
}

void GamePersistence::save(const GameState& state, const std::string& path) const {
	try {
		std::ostringstream dataBuffer;
		dataBuffer << SAVE_SIGNATURE << '\n';

		dataBuffer << state.levelIndex << ' ' << state.turnCounter << ' '
		    << convertBoolToInt(state.playerActionTaken) << ' '
		    << state.enemiesKilledSinceLastSpell << ' '
		    << state.baseFieldWidth << ' ' << state.baseFieldHeight << ' '
		    << state.currentEnemyHealth << ' ' << state.currentEnemyDamage << '\n';

		writeField(dataBuffer, state.fieldSnapshot);
		writePlayer(dataBuffer, state.playerSnapshot);
		writeSpellStates(dataBuffer, state.availableSpells);

		writeCollection<Enemy::Snapshot>(dataBuffer, state.enemySnapshots, [this](auto& stream, const Enemy::Snapshot& enemy) {
			writeEnemy(stream, enemy);
		});

		writeCollection<EnemyBuilding::Snapshot>(dataBuffer, state.buildingSnapshots, [this](auto& stream, const EnemyBuilding::Snapshot& building) {
			writeBuilding(stream, building);
		});

		writeCollection<EnemyTower::Snapshot>(dataBuffer, state.towerSnapshots, [this](auto& stream, const EnemyTower::Snapshot& tower) {
			writeTower(stream, tower);
		});

		writeCollection<Trap::Snapshot>(dataBuffer, state.trapSnapshots, [this](auto& stream, const Trap::Snapshot& trap) {
			writeTrap(stream, trap);
		});

		writeCollection<Ally::Snapshot>(dataBuffer, state.allySnapshots, [this](auto& stream, const Ally::Snapshot& ally) {
			writeAlly(stream, ally);
		});

		writeSpellEnhancements(dataBuffer, state.permanentSpellEnhancements);

		std::string dataContent = dataBuffer.str();
		
		uint64_t checksum = 0;
		for (char byte : dataContent) {
			checksum += static_cast<unsigned char>(byte);
			checksum = (checksum << 1) | (checksum >> 63);
		}

		std::ofstream out(path, std::ios::binary);
		if (!out.is_open()) {
			throw FileAccessException(path, "open for writing");
		}

		out.write(dataContent.c_str(), dataContent.size());
		if (!out.good()) {
			throw SaveGameException("Failed to write game data to: " + path);
		}

		writeChecksumToStream(out, checksum);
		if (!out.good()) {
			throw SaveGameException("Failed to write checksum to: " + path);
		}

		out.flush();
		if (!out.good()) {
			throw SaveGameException("Failed to flush data to: " + path);
		}
	} catch (const SaveGameException&) {
		throw;
	} catch (const FileAccessException&) {
		throw;
	} catch (const std::exception& ex) {
		throw SaveGameException("Unexpected error while saving to " + path + ": " + ex.what());
	}
}

GameState GamePersistence::load(const std::string& path) const {
	std::ifstream in(path, std::ios::binary);
	if (!in.is_open()) {
		throw FileAccessException(path, "open for reading");
	}

	try {
		in.seekg(0, std::ios::end);
		std::streampos fileSize = in.tellg();
		in.seekg(0, std::ios::beg);
		
		constexpr size_t checksumSize = sizeof(uint64_t);
		if (fileSize < static_cast<std::streampos>(checksumSize)) {
			throw CorruptedDataException("file is too small to contain checksum");
		}
		
		std::streampos dataEndPos = fileSize - static_cast<std::streampos>(checksumSize);
		
		std::string fileContent;
		fileContent.resize(static_cast<size_t>(dataEndPos));
		in.read(&fileContent[0], dataEndPos);
		
		if (static_cast<std::streampos>(in.gcount()) != dataEndPos) {
			throw CorruptedDataException("failed to read file data");
		}
		
		uint64_t storedChecksum = readChecksumFromStream(in);
		
		uint64_t calculatedChecksum = 0;
		for (char byte : fileContent) {
			calculatedChecksum += static_cast<unsigned char>(byte);
			calculatedChecksum = (calculatedChecksum << 1) | (calculatedChecksum >> 63);
		}
		
		if (calculatedChecksum != storedChecksum) {
			throw FileModificationException(path, storedChecksum, calculatedChecksum);
		}
		
		std::istringstream dataStream(fileContent);
		
		std::string signature;
		dataStream >> signature;
		if (dataStream.fail() || dataStream.eof()) {
			throw CorruptedDataException("file signature");
		}
		if (signature != SAVE_SIGNATURE) {
			throw InvalidDataException("file signature", 
				"expected '" + std::string(SAVE_SIGNATURE) + "', got '" + signature + "'");
		}

		GameState state;
		int actionTaken = 0;
		dataStream >> state.levelIndex >> state.turnCounter >> actionTaken >> state.enemiesKilledSinceLastSpell
		   >> state.baseFieldWidth >> state.baseFieldHeight >> state.currentEnemyHealth >> state.currentEnemyDamage;
		
		if (dataStream.fail() || dataStream.eof()) {
			throw CorruptedDataException("game metadata");
		}

		if (state.levelIndex < 1 || state.levelIndex > 100) {
			throw InvalidDataException("level index", 
				"value " + std::to_string(state.levelIndex) + " is out of valid range [1, 100]");
		}
		if (state.turnCounter < 0) {
			throw InvalidDataException("turn counter", 
				"negative value: " + std::to_string(state.turnCounter));
		}
		if (state.baseFieldWidth < 10 || state.baseFieldWidth > 30 || 
		    state.baseFieldHeight < 10 || state.baseFieldHeight > 30) {
			throw InvalidDataException("field dimensions", 
				"width=" + std::to_string(state.baseFieldWidth) + 
				" height=" + std::to_string(state.baseFieldHeight) + 
				" (valid range: 10-30)");
		}
		if (state.currentEnemyHealth <= 0 || state.currentEnemyHealth > 10000) {
			throw InvalidDataException("current enemy health", 
				"value: " + std::to_string(state.currentEnemyHealth));
		}
		if (state.currentEnemyDamage < 0 || state.currentEnemyDamage > 1000) {
			throw InvalidDataException("current enemy damage", 
				"value: " + std::to_string(state.currentEnemyDamage));
		}

		state.playerActionTaken = convertIntToBool(actionTaken);

		state.fieldSnapshot = readField(dataStream);
		if (dataStream.fail() || dataStream.eof()) {
			throw CorruptedDataException("field data");
		}

		state.playerSnapshot = readPlayer(dataStream);
		if (dataStream.fail() || dataStream.eof()) {
			throw CorruptedDataException("player data");
		}

		state.availableSpells = readSpellStates(dataStream);

		state.enemySnapshots = readCollection<Enemy::Snapshot>(dataStream, [this](auto& stream) {
			return readEnemy(stream);
		});
		if (dataStream.fail()) {
			throw CorruptedDataException("enemies data");
		}

		state.buildingSnapshots = readCollection<EnemyBuilding::Snapshot>(dataStream, [this](auto& stream) {
			return readBuilding(stream);
		});
		if (dataStream.fail()) {
			throw CorruptedDataException("buildings data");
		}

		state.towerSnapshots = readCollection<EnemyTower::Snapshot>(dataStream, [this](auto& stream) {
			return readTower(stream);
		});
		if (dataStream.fail()) {
			throw CorruptedDataException("towers data");
		}

		state.trapSnapshots = readCollection<Trap::Snapshot>(dataStream, [this](auto& stream) {
			return readTrap(stream);
		});
		if (dataStream.fail()) {
			throw CorruptedDataException("traps data");
		}

		state.allySnapshots = readCollection<Ally::Snapshot>(dataStream, [this](auto& stream) {
			return readAlly(stream);
		});
		if (dataStream.fail()) {
			throw CorruptedDataException("allies data");
		}

		if (!dataStream.eof()) {
			try {
				state.permanentSpellEnhancements = readSpellEnhancements(dataStream);
			} catch (const CorruptedDataException&) {
				if (!dataStream.eof()) {
					throw;
				}
			}
		}

		if (!dataStream.eof()) {
			char remaining;
			dataStream >> remaining;
			if (!dataStream.eof() && !dataStream.fail()) {
				throw CorruptedDataException("unexpected data at end of file");
			}
		}

		return state;
	} catch (const LoadGameException&) {
		throw;
	} catch (const std::exception& ex) {
		throw LoadGameException("Unexpected error while loading from " + path + ": " + ex.what());
	}
}


