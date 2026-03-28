#include "save_manager.hpp"
#include "exceptions.hpp"

#include <fstream>
#include <iomanip>

namespace {
	void ExpectTag(std::istream& in, const std::string& expected) {
		std::string tag;
		if (!(in >> tag) || tag != expected) {
			throw SaveFormatError("expected " + expected);
		}
	}
}

SaveManager::SaveManager(const std::string& fileName)
	: fileName_(fileName) {}

void SaveManager::save(const SaveData& data) const {
	std::ofstream out(fileName_, std::ios::trunc);
	if (!out) {
		throw SaveFileError("cannot open file '" + fileName_ + "' for writing");
	}

	out << "VERSION " << data.fileVersion << "\n";
	out << "ACTIVE " << (data.hasActiveGame ? 1 : 0) << "\n";
	if (data.hasActiveGame) {
		out << "LEVEL " << data.currentLevelIndex << "\n";
		out << "SPELL_POWER " << data.spellPowerBonus << "\n";
		out << "BASE "
			<< data.baseWidth << " "
			<< data.baseHeight << " "
			<< data.baseWallDensity << " "
			<< data.baseEnemyCount << " "
			<< data.baseEnemyHp << " "
			<< data.baseEnemyDamage << "\n";
		out << "BOARD " << data.boardWidth << " " << data.boardHeight << " " << data.wallDensity << "\n";
		out << "PLAYER_NAME " << std::quoted(data.playerName) << "\n";
		out << "PLAYER_STATS "
			<< data.playerCurrentHealth << " "
			<< data.playerMaxHealth << " "
			<< data.playerMeleeDamage << " "
			<< data.playerRangedDamage << " "
			<< data.playerRangedRange << " "
			<< data.playerScore << " "
			<< data.playerMode << " "
			<< data.playerX << " "
			<< data.playerY << " "
			<< data.killsSinceSpell << "\n";

		out << "HAND " << data.serializedSpells.size() << "\n";
		for (const auto& code : data.serializedSpells) {
			out << "CARD " << code << "\n";
		}

		out << "BOARD_CELLS " << data.boardCells.size() << "\n";
		for (std::size_t i = 0; i < data.boardCells.size(); ++i) {
			out << data.boardCells[i];
			if (i + 1 < data.boardCells.size()) out << ' ';
		}
		out << "\n";

		out << "ENEMIES " << data.enemies.size() << "\n";
		for (const auto& enemy : data.enemies) {
			out << "ENEMY " << enemy.hp << " "
				<< enemy.damage << " "
				<< enemy.x << " "
				<< enemy.y << "\n";
		}

		out << "TOWERS " << data.towers.size() << "\n";
		for (const auto& tower : data.towers) {
			out << "TOWER " << tower.hp << " "
				<< tower.damage << " "
				<< tower.x << " "
				<< tower.y << " "
				<< tower.range << " "
				<< tower.cooldown << "\n";
		}

		out << "TRAPS " << data.traps.size() << "\n";
		for (const auto& trap : data.traps) {
			out << "TRAP " << trap.x << " "
				<< trap.y << " "
				<< trap.damage << "\n";
		}

		out << "SPELL_RNG " << std::quoted(data.spellRngState) << "\n";
		out << "CTRL_RNG " << std::quoted(data.controllerRngState) << "\n";
	}

	out.close();
	if (!out) {
		throw SaveFileError("failed while writing to '" + fileName_ + "'");
	}
}

SaveData SaveManager::load() const {
	std::ifstream in(fileName_);
	if (!in) {
		throw SaveFileError("cannot open file '" + fileName_ + "' for reading");
	}

	SaveData data;
	std::string tag;

	if (!(in >> tag) || tag != "VERSION") {
		throw SaveFormatError("expected VERSION");
	}
	in >> data.fileVersion;

	if (!(in >> tag) || tag != "ACTIVE") {
		throw SaveFormatError("expected ACTIVE");
	}
	int active = 0;
	in >> active;
	data.hasActiveGame = (active != 0);

	if (!data.hasActiveGame) {
		return data;
	}

	ExpectTag(in, "LEVEL");
	in >> data.currentLevelIndex;
	if (data.currentLevelIndex < 1) {
		throw SaveFormatError("level index must be >= 1");
	}

	ExpectTag(in, "SPELL_POWER");
	in >> data.spellPowerBonus;
	if (data.spellPowerBonus < 0 || data.spellPowerBonus > 100000) {
		throw SaveFormatError("invalid spell power bonus");
	}

	ExpectTag(in, "BASE");
	in >> data.baseWidth
	   >> data.baseHeight
	   >> data.baseWallDensity
	   >> data.baseEnemyCount
	   >> data.baseEnemyHp
	   >> data.baseEnemyDamage;
	if (data.baseWidth < 10 || data.baseWidth > 25 ||
	    data.baseHeight < 10 || data.baseHeight > 25) {
		throw SaveFormatError("base board size out of bounds");
	}
	if (data.baseWallDensity < 0.0 || data.baseWallDensity > 0.5) {
		throw SaveFormatError("base wall density out of bounds");
	}
	if (data.baseEnemyCount < 0 || data.baseEnemyCount > 100000) {
		throw SaveFormatError("base enemy count out of bounds");
	}
	if (data.baseEnemyHp < 1 || data.baseEnemyHp > 100000) {
		throw SaveFormatError("base enemy hp out of bounds");
	}
	if (data.baseEnemyDamage < 0 || data.baseEnemyDamage > 10000) {
		throw SaveFormatError("base enemy damage out of bounds");
	}

	ExpectTag(in, "BOARD");
	in >> data.boardWidth >> data.boardHeight >> data.wallDensity;
	if (data.boardWidth < 10 || data.boardWidth > 25 ||
	    data.boardHeight < 10 || data.boardHeight > 25) {
		throw SaveFormatError("board size out of bounds");
	}
	if (data.wallDensity < 0.0 || data.wallDensity > 0.5) {
		throw SaveFormatError("wall density out of bounds");
	}

	ExpectTag(in, "PLAYER_NAME");
	in >> std::quoted(data.playerName);
	if (data.playerName.empty()) {
		throw SaveFormatError("player name empty");
	}

	ExpectTag(in, "PLAYER_STATS");
	in >> data.playerCurrentHealth
	   >> data.playerMaxHealth
	   >> data.playerMeleeDamage
	   >> data.playerRangedDamage
	   >> data.playerRangedRange
	   >> data.playerScore
	   >> data.playerMode
	   >> data.playerX
	   >> data.playerY
	   >> data.killsSinceSpell;
	if (data.playerMaxHealth < 1 || data.playerMaxHealth > 100000) {
		throw SaveFormatError("player max hp out of bounds");
	}
	if (data.playerCurrentHealth < 0 || data.playerCurrentHealth > data.playerMaxHealth) {
		throw SaveFormatError("player current hp out of bounds");
	}
	if (data.playerMeleeDamage < 0 || data.playerMeleeDamage > 10000 ||
	    data.playerRangedDamage < 0 || data.playerRangedDamage > 10000) {
		throw SaveFormatError("player damage out of bounds");
	}
	if (data.playerRangedRange < 1 || data.playerRangedRange > 10) {
		throw SaveFormatError("player ranged range out of bounds");
	}
	if (data.playerScore < 0 || data.playerScore > 100000000) {
		throw SaveFormatError("player score out of bounds");
	}
	if (data.playerMode != 0 && data.playerMode != 1) {
		throw SaveFormatError("invalid player mode");
	}
	if (data.playerX < 0 || data.playerX >= data.boardWidth ||
	    data.playerY < 0 || data.playerY >= data.boardHeight) {
		throw SaveFormatError("player position out of bounds");
	}
	if (data.killsSinceSpell < 0 || data.killsSinceSpell > 1000000) {
		throw SaveFormatError("invalid kills counter");
	}

	ExpectTag(in, "HAND");
	std::size_t handCount = 0;
	in >> handCount;
	if (handCount > 1000) {
		throw SaveFormatError("hand too large");
	}
	data.serializedSpells.clear();
	data.serializedSpells.reserve(handCount);
	for (std::size_t i = 0; i < handCount; ++i) {
		ExpectTag(in, "CARD");
		std::string code;
		in >> code;
		data.serializedSpells.push_back(code);
	}

	ExpectTag(in, "BOARD_CELLS");
	std::size_t cellCount = 0;
	in >> cellCount;
	std::size_t expectedCells = static_cast<std::size_t>(data.boardWidth) * static_cast<std::size_t>(data.boardHeight);
	if (cellCount != expectedCells) {
		throw SaveFormatError("board cell count mismatch");
	}
	data.boardCells.assign(cellCount, 0);
	for (std::size_t i = 0; i < cellCount; ++i) {
		in >> data.boardCells[i];
		if (data.boardCells[i] != 0 && data.boardCells[i] != 1) {
			throw SaveFormatError("invalid cell value");
		}
	}

	ExpectTag(in, "ENEMIES");
	std::size_t enemyCount = 0;
	in >> enemyCount;
	if (enemyCount > expectedCells) {
		throw SaveFormatError("too many enemies");
	}
	data.enemies.clear();
	data.enemies.reserve(enemyCount);
	for (std::size_t i = 0; i < enemyCount; ++i) {
		ExpectTag(in, "ENEMY");
		SavedEnemy enemy;
		in >> enemy.hp >> enemy.damage >> enemy.x >> enemy.y;
		if (enemy.hp < 1 || enemy.hp > 100000 || enemy.damage < 0 || enemy.damage > 10000) {
			throw SaveFormatError("enemy stats out of bounds");
		}
		if (enemy.x < 0 || enemy.x >= data.boardWidth || enemy.y < 0 || enemy.y >= data.boardHeight) {
			throw SaveFormatError("enemy position out of bounds");
		}
		data.enemies.push_back(enemy);
	}

	ExpectTag(in, "TOWERS");
	std::size_t towerCount = 0;
	in >> towerCount;
	if (towerCount > expectedCells) {
		throw SaveFormatError("too many towers");
	}
	data.towers.clear();
	data.towers.reserve(towerCount);
	for (std::size_t i = 0; i < towerCount; ++i) {
		ExpectTag(in, "TOWER");
		SavedTower tower;
		in >> tower.hp >> tower.damage >> tower.x >> tower.y >> tower.range >> tower.cooldown;
		if (tower.hp < 1 || tower.hp > 100000 || tower.damage < 0 || tower.damage > 10000) {
			throw SaveFormatError("tower stats out of bounds");
		}
		if (tower.range < 0 || tower.range > 25 || tower.cooldown < 0 || tower.cooldown > 1000) {
			throw SaveFormatError("tower range/cooldown out of bounds");
		}
		if (tower.x < 0 || tower.x >= data.boardWidth || tower.y < 0 || tower.y >= data.boardHeight) {
			throw SaveFormatError("tower position out of bounds");
		}
		data.towers.push_back(tower);
	}

	ExpectTag(in, "TRAPS");
	std::size_t trapCount = 0;
	in >> trapCount;
	if (trapCount > expectedCells) {
		throw SaveFormatError("too many traps");
	}
	data.traps.clear();
	data.traps.reserve(trapCount);
	for (std::size_t i = 0; i < trapCount; ++i) {
		ExpectTag(in, "TRAP");
		SavedTrap trap;
		in >> trap.x >> trap.y >> trap.damage;
		if (trap.damage < 0 || trap.damage > 10000) {
			throw SaveFormatError("trap damage out of bounds");
		}
		if (trap.x < 0 || trap.x >= data.boardWidth || trap.y < 0 || trap.y >= data.boardHeight) {
			throw SaveFormatError("trap position out of bounds");
		}
		data.traps.push_back(trap);
	}

	ExpectTag(in, "SPELL_RNG");
	in >> std::quoted(data.spellRngState);

	ExpectTag(in, "CTRL_RNG");
	in >> std::quoted(data.controllerRngState);

	in.close();
	if (!in) {
		throw SaveFormatError("unexpected EOF or parse error");
	}
	
	return data;
}
