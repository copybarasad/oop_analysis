#ifndef SAVE_DATA_VALIDATOR_H
#define SAVE_DATA_VALIDATOR_H

#include "position.h"
#include "game_field.h"
#include "game_exceptions.h"
#include <string>
#include <vector>

class SaveDataValidator {
public:
	static void validatePosition(const Position& pos, const GameField& field, const std::string& context);
	static void validateHealth(int health, const std::string& context);
	static void validateMana(int mana, int maxMana, const std::string& context);
	static void validateDamage(int damage, const std::string& context);
	static void validateScore(int score, const std::string& context);
	static void validateRange(int range, const std::string& context);
	static std::vector<int> parseCommaSeparatedValues(const std::string& line, int expectedCount, const std::string& context);
};

#endif





