#include "game_save_manager.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include "output_file_raii.h"
#include "input_file_raii.h"
#include "game_exceptions.h"
#include "save_data_validator.h"
#include <filesystem>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace fs = std::filesystem;

GameSaveManager::GameSaveManager(const std::string& saveDir)
	: saveDirectory(saveDir) {}

void GameSaveManager::saveGame(const std::string& filename, 
	                          int level,
	                          const Player& player, 
	                          const GameField& field,
	                          const EnemyManager& enemyManager,
	                          const BuildingManager& buildingManager,
	                          const CombatManager& combatManager,
	                          const std::vector<Tower>& towers) const {
	
	std::ofstream file(filename);
	if (!file.is_open()) throw GameSaveException("Cannot open file: " + filename);
	OutputFileRAII fileGuard(file);
	
	file << "[Meta]" << std::endl;
	file << "Version=2" << std::endl;
	file << "Level=" << level << std::endl << std::endl;
	
	file << "[Player]" << std::endl;
	file << "Health=" << player.getHealth() << std::endl;
	file << "Mana=" << player.getMana() << std::endl;
	file << "Score=" << player.getScore() << std::endl;
	file << "CombatMode=" << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") << std::endl;
	file << "Position=" << player.getPosition().getX() << "," << player.getPosition().getY() << std::endl;
	file << "CanMove=" << (player.getCanMove() ? "true" : "false") << std::endl << std::endl;
	
	file << "[Field]" << std::endl;
	file << "Width=" << field.getWidth() << std::endl;
	file << "Height=" << field.getHeight() << std::endl;
	file << "Grid=";
	for (int y = 0; y < field.getHeight(); ++y) {
	    for (int x = 0; x < field.getWidth(); ++x) {
	        file << cellTypeToChar(field.getCell(Position(x, y)).getType());
	    }
	    if (y < field.getHeight() - 1) file << ";";
	}
	file << std::endl << std::endl;
	
	file << "[Enemies]" << std::endl;
	for (const auto& enemy : enemyManager.getEnemies()) {
	    file << enemy.getHealth() << "," << enemy.getDamage() << "," 
	         << enemy.getPosition().getX() << "," << enemy.getPosition().getY() << std::endl;
	}
	file << std::endl;
	
	file << "[Buildings]" << std::endl;
	for (const auto& building : buildingManager.getBuildings()) {
	    file << building.getPosition().getX() << "," << building.getPosition().getY() << ",3,0" << std::endl;
	}
	file << std::endl;
	
	file << "[Towers]" << std::endl;
	for (const auto& tower : towers) {
	    file << tower.getPosition().getX() << "," << tower.getPosition().getY() << ","
	         << tower.getRange() << ",2,0" << std::endl;
	}
	file << std::endl;
	
	file << "[Traps]" << std::endl;
	for (const auto& trap : combatManager.getActiveTraps()) {
	    file << trap.getX() << "," << trap.getY() << std::endl;
	}
	file << std::endl;
	
	file << "[Spells]" << std::endl;
	const auto& spellHand = player.getSpellHand();
	for (int i = 0; i < spellHand.getSpellCount(); ++i) {
	    SpellCard* spell = spellHand.getSpell(i);
	    if (spell) {
	        file << spellTypeToString(spell) << ",";
	        if (auto* directSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
	            file << directSpell->getDamage() << "," << directSpell->getRange();
	        } else if (auto* areaSpell = dynamic_cast<AreaDamageSpell*>(spell)) {
	            file << areaSpell->getDamage() << "," << areaSpell->getRange();
	        } else if (auto* trapSpell = dynamic_cast<TrapSpell*>(spell)) {
	            file << trapSpell->getDamage() << "," << trapSpell->getRange();
	        }
	        file << std::endl;
	    }
	}
}

bool GameSaveManager::loadGame(const std::string& filename,
	                          int& outLevel,
	                          Player& player,
	                          GameField& field,
	                          EnemyManager& enemyManager,
	                          BuildingManager& buildingManager,
	                          CombatManager& combatManager,
	                          std::vector<Tower>& towers) const {
	
	if (!saveFileExists(filename)) throw GameLoadException("Save file does not exist: " + filename);
	
	std::ifstream file(filename);
	if (!file.is_open()) throw GameLoadException("Cannot open save file: " + filename);
	InputFileRAII fileGuard(file);
	
	outLevel = 1;
	std::vector<std::string> allLines;
	std::string line;
	while (std::getline(file, line)) {
		allLines.push_back(line);
	}

	std::string section;
	int savedWidth = -1;
	int savedHeight = -1;
	std::string savedGrid;

	for (const auto& rawLine : allLines) {
	    if (rawLine.empty() || rawLine[0] == '#') continue;

	    if (rawLine[0] == '[') {
	        size_t endBracket = rawLine.find(']');
	        if (endBracket == std::string::npos) throw InvalidSaveDataException("Invalid section format");
	        section = rawLine.substr(1, endBracket - 1);
	        continue;
	    }

	    size_t eqPos = rawLine.find('=');
	    if (eqPos == std::string::npos) continue;

	    std::string key = rawLine.substr(0, eqPos);
	    std::string value = rawLine.substr(eqPos + 1);

	    if (section == "Meta") {
	        if (key == "Level") {
	            try {
	                outLevel = std::stoi(value);
	            } catch (const std::exception&) {
	                throw InvalidSaveDataException("Invalid Level value");
	            }
	        }
	    } else if (section == "Field") {
	        if (key == "Width") {
	            savedWidth = std::stoi(value);
	        } else if (key == "Height") {
	            savedHeight = std::stoi(value);
	        } else if (key == "Grid") {
	            savedGrid = value;
	        }
	    }
	}

	if (savedWidth <= 0 || savedHeight <= 0) {
	    throw InvalidSaveDataException("Missing field dimensions in save file");
	}

	field = GameField(savedWidth, savedHeight);
	field.clearAllOccupancy();

	if (!savedGrid.empty()) {
	    std::vector<std::string> rows;
	    std::string current;
	    for (char c : savedGrid) {
	        if (c == ';') {
	            rows.push_back(current);
	            current.clear();
	        } else {
	            current.push_back(c);
	        }
	    }
	    rows.push_back(current);

	    if (static_cast<int>(rows.size()) != savedHeight) {
	        throw InvalidSaveDataException("Field grid height mismatch");
	    }

	    for (int y = 0; y < savedHeight; ++y) {
	        if (static_cast<int>(rows[y].size()) != savedWidth) {
	            throw InvalidSaveDataException("Field grid width mismatch");
	        }
	        for (int x = 0; x < savedWidth; ++x) {
	            field.setCellType(Position(x, y), charToCellType(rows[y][x]));
	        }
	    }
	}

	enemyManager.clearEnemies();
	buildingManager.clearBuildings();
	towers.clear();
	combatManager.clearTraps();

	auto& spellHand = player.getSpellHand();
	while (spellHand.getSpellCount() > 0) spellHand.removeSpell(0);

	bool hasPlayerSection = false, hasFieldSection = false;
	section.clear();

	for (const auto& rawLine : allLines) {
	    if (rawLine.empty() || rawLine[0] == '#') continue;

	    if (rawLine[0] == '[') {
	        size_t endBracket = rawLine.find(']');
	        if (endBracket == std::string::npos) throw InvalidSaveDataException("Invalid section format");
	        section = rawLine.substr(1, endBracket - 1);
	        if (section == "Player") hasPlayerSection = true;
	        if (section == "Field") hasFieldSection = true;
	        continue;
	    }

	    std::istringstream iss(rawLine);

	    if (section == "Player") {
	        size_t eqPos = rawLine.find('=');
	        if (eqPos == std::string::npos) throw InvalidSaveDataException("Invalid player data format");

	        std::string key = rawLine.substr(0, eqPos);
	        std::string value = rawLine.substr(eqPos + 1);

	        if (key == "Health") {
	            int targetHealth = std::stoi(value);
	            SaveDataValidator::validateHealth(targetHealth, "Player");
	            int currentHealth = player.getHealth();
	            if (targetHealth < currentHealth) player.takeDamage(currentHealth - targetHealth);
	            else if (targetHealth > currentHealth) player.heal(targetHealth - currentHealth);
	        } else if (key == "Mana") {
	            int mana = std::stoi(value);
	            SaveDataValidator::validateMana(mana, player.getMaxMana(), "Player");
	            player.setMana(mana);
	        } else if (key == "Score") {
	            int targetScore = std::stoi(value);
	            SaveDataValidator::validateScore(targetScore, "Player");
	            player.setScore(targetScore);
	        } else if (key == "CombatMode") {
	            if (value == "RANGED" && player.getCombatMode() == CombatMode::MELEE) player.switchCombatMode();
	            else if (value == "MELEE" && player.getCombatMode() == CombatMode::RANGED) player.switchCombatMode();
	            else if (value != "MELEE" && value != "RANGED") throw InvalidSaveDataException("Invalid combat mode");
	        } else if (key == "Position") {
	            auto values = SaveDataValidator::parseCommaSeparatedValues(value, 2, "Player position");
	            Position newPos(values[0], values[1]);
	            SaveDataValidator::validatePosition(newPos, field, "Player");
	            if (!field.isPositionPassable(newPos)) throw InvalidSaveDataException("Player position not passable");
	            player.setPosition(newPos);
	            field.setCellOccupied(newPos, true);
	        } else if (key == "CanMove") {
	            if (value != "true" && value != "false") throw InvalidSaveDataException("Invalid CanMove value");
	            player.setCanMove(value == "true");
	        }
	    }
	    else if (section == "Enemies") {
	        auto values = SaveDataValidator::parseCommaSeparatedValues(rawLine, 4, "Enemy data");
	        SaveDataValidator::validateHealth(values[0], "Enemy");
	        SaveDataValidator::validateDamage(values[1], "Enemy");
	        Position pos(values[2], values[3]);
	        SaveDataValidator::validatePosition(pos, field, "Enemy");
	        if (!field.isPositionPassable(pos)) throw InvalidSaveDataException("Enemy position not passable");
	        if (pos == player.getPosition()) throw InvalidSaveDataException("Enemy overlaps with player");

	        Enemy enemy(values[0], values[1]);
	        enemy.setPosition(pos);
	        enemyManager.addEnemy(enemy);
	    }
	    else if (section == "Buildings") {
	        auto values = SaveDataValidator::parseCommaSeparatedValues(rawLine, 4, "Building data");
	        Position pos(values[0], values[1]);
	        SaveDataValidator::validatePosition(pos, field, "Building");
	        if (!field.isPositionPassable(pos)) throw InvalidSaveDataException("Building position not passable");
	        if (pos == player.getPosition()) throw InvalidSaveDataException("Building overlaps with player");

	        EnemyBuilding building(3);
	        building.setPosition(pos);
	        buildingManager.addBuilding(building);
	    }
	    else if (section == "Towers") {
	        auto values = SaveDataValidator::parseCommaSeparatedValues(rawLine, 5, "Tower data");
	        SaveDataValidator::validateRange(values[2], "Tower");
	        Position pos(values[0], values[1]);
	        SaveDataValidator::validatePosition(pos, field, "Tower");
	        if (!field.isPositionPassable(pos)) throw InvalidSaveDataException("Tower position not passable");
	        if (pos == player.getPosition()) throw InvalidSaveDataException("Tower overlaps with player");

	        Tower tower(values[2], 2);
	        tower.setPosition(pos);
	        towers.push_back(tower);
	        field.setCellOccupied(pos, true);
	    }
	    else if (section == "Traps") {
	        auto values = SaveDataValidator::parseCommaSeparatedValues(rawLine, 2, "Trap data");
	        Position pos(values[0], values[1]);
	        SaveDataValidator::validatePosition(pos, field, "Trap");
	        combatManager.getActiveTraps().push_back(pos);
	    }
	    else if (section == "Spells") {
	        std::vector<std::string> values;
	        std::string token;
	        while (std::getline(iss, token, ',')) values.push_back(token);
	        if (values.size() != 3) throw InvalidSaveDataException("Invalid spell data format");

	        auto spell = stringToSpell(values[0], std::stoi(values[1]), std::stoi(values[2]));
	        if (spell) spellHand.addSpell(std::move(spell));
	        else throw InvalidSaveDataException("Unknown spell type: " + values[0]);
	    }
	    // ignore other sections/keys for forward/backward compatibility
	}
	
	if (!hasPlayerSection) throw InvalidSaveDataException("Missing [Player] section");
	if (!hasFieldSection) throw InvalidSaveDataException("Missing [Field] section");
	
	SaveDataValidator::validateHealth(player.getHealth(), "Player final");
	SaveDataValidator::validateMana(player.getMana(), player.getMaxMana(), "Player final");
	SaveDataValidator::validateScore(player.getScore(), "Player final");
	
	return true;
}

std::vector<std::string> GameSaveManager::getSaveFiles() const {
	std::vector<std::string> saveFiles;
	for (const auto& entry : fs::directory_iterator(saveDirectory)) {
	    if (entry.is_regular_file() && entry.path().extension() == ".sav") {
	        saveFiles.push_back(entry.path().filename().string());
	    }
	}
	std::sort(saveFiles.begin(), saveFiles.end(), [](const std::string& a, const std::string& b) {
	    return fs::last_write_time(a) > fs::last_write_time(b);
	});
	return saveFiles;
}

bool GameSaveManager::saveFileExists(const std::string& filename) const {
	return fs::exists(filename);
}

CellType GameSaveManager::charToCellType(char c) const {
	switch (c) {
	    case 'O': return CellType::OBSTACLE;
	    case 'S': return CellType::SLOW;
	    case 'E': return CellType::EMPTY;
	    default: throw InvalidSaveDataException("Invalid cell type character");
	}
}

char GameSaveManager::cellTypeToChar(CellType type) const {
	switch (type) {
	    case CellType::OBSTACLE: return 'O';
	    case CellType::SLOW: return 'S';
	    case CellType::EMPTY: return 'E';
	    default: return 'E';
	}
}

std::string GameSaveManager::spellTypeToString(SpellCard* spell) const {
	if (dynamic_cast<DirectDamageSpell*>(spell)) return "DIRECT";
	if (dynamic_cast<AreaDamageSpell*>(spell)) return "AREA";
	if (dynamic_cast<TrapSpell*>(spell)) return "TRAP";
	throw InvalidSaveDataException("Unknown spell type");
}

std::unique_ptr<SpellCard> GameSaveManager::stringToSpell(const std::string& type, int damage, int range) const {
	if (type == "DIRECT") return std::make_unique<DirectDamageSpell>(damage, range);
	if (type == "AREA") return std::make_unique<AreaDamageSpell>(damage, range);
	if (type == "TRAP") return std::make_unique<TrapSpell>(damage, range);
	return nullptr;
}