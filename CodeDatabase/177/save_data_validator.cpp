#include "save_data_validator.h"
#include <sstream>
#include <stdexcept>

void SaveDataValidator::validatePosition(const Position& pos, const GameField& field, const std::string& context) {
	if (pos.getX() < 0 || pos.getX() >= field.getWidth() || 
	    pos.getY() < 0 || pos.getY() >= field.getHeight()) {
		throw InvalidSaveDataException(context + " position out of bounds");
	}
}

void SaveDataValidator::validateHealth(int health, const std::string& context) {
	if (health < 0 || health > 10000) {
		throw InvalidSaveDataException(context + " health invalid: " + std::to_string(health));
	}
}

void SaveDataValidator::validateMana(int mana, int maxMana, const std::string& context) {
	if (mana < 0 || mana > maxMana) {
		throw InvalidSaveDataException(context + " mana invalid: " + std::to_string(mana));
	}
}

void SaveDataValidator::validateDamage(int damage, const std::string& context) {
	if (damage < 0 || damage > 1000) {
		throw InvalidSaveDataException(context + " damage invalid: " + std::to_string(damage));
	}
}

void SaveDataValidator::validateScore(int score, const std::string& context) {
	if (score < 0 || score > 1000000) {
		throw InvalidSaveDataException(context + " score invalid: " + std::to_string(score));
	}
}

void SaveDataValidator::validateRange(int range, const std::string& context) {
	if (range < 0 || range > 20) {
		throw InvalidSaveDataException(context + " range invalid: " + std::to_string(range));
	}
}

std::vector<int> SaveDataValidator::parseCommaSeparatedValues(const std::string& line, int expectedCount, const std::string& context) {
	std::vector<int> values;
	std::istringstream iss(line);
	std::string token;
	
	while (std::getline(iss, token, ',')) {
		try {
			values.push_back(std::stoi(token));
		} catch (const std::exception&) {
			throw InvalidSaveDataException(context + " invalid number format");
		}
	}
	
	if (values.size() != static_cast<size_t>(expectedCount)) {
		throw InvalidSaveDataException(context + " expected " + std::to_string(expectedCount) + " values");
	}
	
	return values;
}





