#ifndef GAME_SAVE_MANAGER_H
#define GAME_SAVE_MANAGER_H

#include "player.h"
#include "game_field.h"
#include "enemy_manager.h"
#include "building_manager.h"
#include "combat_manager.h"
#include "tower.h"
#include <vector>
#include <string>

class GameSaveManager {
private:
	std::string saveDirectory;
	
	CellType charToCellType(char c) const;
	char cellTypeToChar(CellType type) const;
	std::string spellTypeToString(SpellCard* spell) const;
	std::unique_ptr<SpellCard> stringToSpell(const std::string& type, int damage, int range) const;
	
public:
	/**
	 * Конструктор
	 * @param saveDir Директория для сохранения файлов (по умолчанию текущая)
	 */
	explicit GameSaveManager(const std::string& saveDir = ".");
	
	void saveGame(const std::string& filename,
	              int level,
	              const Player& player, 
	              const GameField& field,
	              const EnemyManager& enemyManager,
	              const BuildingManager& buildingManager,
	              const CombatManager& combatManager,
	              const std::vector<Tower>& towers) const;
	
	bool loadGame(const std::string& filename,
	              int& outLevel,
	              Player& player,
	              GameField& field,
	              EnemyManager& enemyManager,
	              BuildingManager& buildingManager,
	              CombatManager& combatManager,
	              std::vector<Tower>& towers) const;
	
	std::vector<std::string> getSaveFiles() const;
	bool saveFileExists(const std::string& filename) const;
};

#endif