#ifndef GAME_PERSISTENCE_H
#define GAME_PERSISTENCE_H

#include <string>
#include "GameSnapshot.h"

class GamePersistence {
public:
	GamePersistence() = default;

	void save(const GameSnapshot& snapshot, const std::string& filePath) const;
	GameSnapshot load(const std::string& filePath) const;
	bool hasSave(const std::string& filePath) const;

private:
	void ensureDirectoryExists(const std::string& filePath) const;
	static int serializeAttackType(AttackType type);
	static AttackType deserializeAttackType(int value);
	static int serializeCellType(CellType type);
	static CellType deserializeCellType(int value);
	static int serializeSpellId(SpellId id);
	static SpellId deserializeSpellId(int value);
};

#endif

