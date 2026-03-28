#ifndef GAME_PERSISTENCE_H
#define GAME_PERSISTENCE_H

#include "game_state.h"
#include "save_game_exception.h"
#include "load_game_exception.h"
#include "file_access_exception.h"
#include "corrupted_data_exception.h"
#include "invalid_data_exception.h"
#include <cstdint>
#include <fstream>

class GamePersistence {
public:
	void save(const GameState& state, const std::string& path) const;
	GameState load(const std::string& path) const;

private:
	uint64_t calculateChecksum(const std::string& filePath) const;
	uint64_t calculateChecksumFromStream(std::ifstream& stream, std::streampos startPos, std::streampos endPos) const;
	void writeChecksumToStream(std::ofstream& stream, uint64_t checksum) const;
	uint64_t readChecksumFromStream(std::ifstream& stream) const;
	
	int convertBoolToInt(bool value) const;
	bool convertIntToBool(int value) const;
	int convertCombatModeToInt(CombatMode mode) const;
	CombatMode convertIntToCombatMode(int value) const;
	
	void writeSpellStates(std::ostream& out, const std::vector<SpellState>& spells) const;
	std::vector<SpellState> readSpellStates(std::istream& in) const;
	
	template <typename T, typename Writer>
	void writeCollection(std::ostream& out, const std::vector<T>& collection, Writer writer) const;
	
	template <typename T, typename Reader>
	std::vector<T> readCollection(std::istream& in, Reader reader) const;
	
	void writeField(std::ostream& out, const Field::Snapshot& field) const;
	Field::Snapshot readField(std::istream& in) const;
	
	void writePlayer(std::ostream& out, const Player::Snapshot& player) const;
	Player::Snapshot readPlayer(std::istream& in) const;
	
	void writeEnemy(std::ostream& out, const Enemy::Snapshot& enemy) const;
	Enemy::Snapshot readEnemy(std::istream& in) const;
	
	void writeBuilding(std::ostream& out, const EnemyBuilding::Snapshot& building) const;
	EnemyBuilding::Snapshot readBuilding(std::istream& in) const;
	
	void writeTower(std::ostream& out, const EnemyTower::Snapshot& tower) const;
	EnemyTower::Snapshot readTower(std::istream& in) const;
	
	void writeTrap(std::ostream& out, const Trap::Snapshot& trap) const;
	Trap::Snapshot readTrap(std::istream& in) const;
	
	void writeAlly(std::ostream& out, const Ally::Snapshot& ally) const;
	Ally::Snapshot readAlly(std::istream& in) const;
	
	void writeSpellEnhancements(std::ostream& out, const std::map<SpellType, int>& enhancements) const;
	std::map<SpellType, int> readSpellEnhancements(std::istream& in) const;
};

#endif

