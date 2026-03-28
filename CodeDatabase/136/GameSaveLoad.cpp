#include "Game.h"
#include "GameExceptions.h"
#include <iostream>
#include <fstream>
#include <memory>

class OutputFileRAII {
private:
    std::ofstream& file;
public:
    OutputFileRAII(std::ofstream& f) : file(f) {}
    ~OutputFileRAII() {
        if (file.is_open()) {
            file.close();
        }
    }
};

bool Game::saveGameToFile(const std::string& filename) {
    if (!currentGame) {
        std::cout << "Warning: No active game to save\n";
        return false;
    }
    
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw FileException(filename, "create/open for writing");
        }
        
        OutputFileRAII fileGuard(file);
        
        file << "BEAST_HUNTER_FULL_SAVE_V2\n";
        
        if (!savePlayerData(file)) {
            throw SaveException("player data", "failed to write player information");
        }
        
        if (!saveFieldData(file)) {
            throw SaveException("field data", "failed to write field layout");
        }
        
        if (!saveEntitiesData(file)) {
            throw SaveException("entities data", "failed to write game entities");
        }
        
        if (file.fail()) {
            throw SaveException("file write", "unknown write error occurred");
        }
        
        std::cout << "Game saved successfully to: " << filename << "\n";
        return true;
        
    } catch (const FileException& e) {
        std::cout << "File error during save: " << e.what() << "\n";
        return false;
    } catch (const SaveException& e) {
        std::cout << "Save error: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "Unexpected error during save: " << e.what() << "\n";
        return false;
    }
}

bool Game::savePlayerData(std::ofstream& file) {
    try {
        Wanderer& player = currentGame->getPlayer();
        
        file << "PLAYER_DATA\n";
        file << player.getHealth() << "\n";
        file << player.getMaxHealth() << "\n";
        file << player.getShield() << "\n";
        file << player.getMaxShield() << "\n";
        file << player.getMana() << "\n";
        file << player.getMaxMana() << "\n";
        file << player.getPoints() << "\n";
        file << player.getLvl() << "\n";
        file << player.getExp() << "\n";
        file << player.getExpToNextLvl() << "\n";
        file << currentGame->getTurnCount() << "\n";
        file << currentGame->getCurrentLevel() << "\n";
        file << (player.getCombatMode() == CombatMode::MELEE ? 0 : 1) << "\n";
        
        Position playerPos = currentGame->getPlayerPosition();
        file << playerPos.x << "\n" << playerPos.y << "\n";
        file << "END_PLAYER_DATA\n";

        if (file.fail()) {
            throw SaveException("player data", "write operation failed");
        }
        
        return true;
        
    } catch (const std::exception& e) {
        throw SaveException("player data", std::string("unexpected: ") + e.what());
    }
}

bool Game::saveFieldData(std::ofstream& file) {
    try {
        GameField& field = currentGame->getField();
        
        file << "FIELD_DATA\n";
        file << field.getWidth() << " " << field.getHeight() << "\n";

        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                const Cell& cell = field.getCell(x, y);
                int cellType = static_cast<int>(cell.getType());
                file << cellType << " ";
                
                if (cell.getHasTrap()) {
                    file << "1 " << cell.getTrapDamage() << " ";
                } else {
                    file << "0 0 ";
                }
                
                if (file.fail()) {
                    throw SaveException("field data", 
                        "write failed at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
                }
            }
            file << "\n";
        }
        file << "END_FIELD_DATA\n";
        
        if (file.fail()) {
            throw SaveException("field data", "write operation failed");
        }
        
        return true;
        
    } catch (const std::exception& e) {
        throw SaveException("field data", std::string("unexpected: ") + e.what());
    }
}

bool Game::saveEntitiesData(std::ofstream& file) {
    try {
        BeastManager& beastManager = currentGame->getBeastManager();
        BuildingManager& buildingManager = currentGame->getBuildingManager();
        
        file << "ENTITIES_DATA\n";
        
        const auto& beasts = beastManager.getBeasts();
        file << "BEASTS " << beasts.size() << "\n";
        for (const auto& beastData : beasts) {
            file << beastData.first.x << " " << beastData.first.y << " " 
                 << beastData.second.getHealth() << " " << beastData.second.getDamage() << "\n";
        }
        
        const auto& buildings = buildingManager.getBuildings();
        file << "BUILDINGS " << buildings.size() << "\n";
        for (const auto& buildingData : buildings) {
            file << buildingData.first.x << " " << buildingData.first.y << " " 
                 << buildingData.second.getHealth() << " " << buildingData.second.getMaxHealth() << "\n";
        }
        
        const auto& towers = buildingManager.getTowers();
        file << "TOWERS " << towers.size() << "\n";
        for (const auto& towerData : towers) {
            file << towerData.first.x << " " << towerData.first.y << " " 
                 << towerData.second.getHealth() << " " << towerData.second.getMaxHealth() << " "
                 << towerData.second.getAttackRange() << "\n";
        }
        
        Wanderer& player = currentGame->getPlayer();
        const Hand& hand = player.getHand();
        file << "SPELLS " << hand.getSpellCount() << "\n";
        for (int i = 0; i < hand.getSpellCount(); ++i) {
            const SpellCard* spell = hand.getSpell(i);
            if (spell) {
                file << spell->getName() << "\n";
            }
        }
        
        file << "END_ENTITIES_DATA\n";
        
        if (file.fail()) {
            throw SaveException("entities data", "write operation failed");
        }
        
        return true;
        
    } catch (const std::exception& e) {
        throw SaveException("entities data", std::string("unexpected: ") + e.what());
    }
}