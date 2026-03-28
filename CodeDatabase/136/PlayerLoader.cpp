#include "PlayerLoader.h"
#include "GameExceptions.h"
#include <iostream>

PlayerLoader::PlayerLoader(std::ifstream& file, Wanderer& player, 
                          GameField& field, GameController& controller)
    : file(file), player(player), field(field), gameController(controller) {}

bool PlayerLoader::loadPlayerData() {
    try {
        return loadPlayerStats();
    } catch (const std::exception& e) {
        throw LoadException("player data", e.what());
    }
}

bool PlayerLoader::loadPlayerStats() {
    try {
        std::string marker;
        file >> marker;
        
        if (marker != "PLAYER_DATA") {
            throw LoadException("player data", "marker not found: " + marker);
        }

        int health, maxHealth, shield, maxShield, mana, maxMana;
        int points, level, exp, expToNext, combatMode;
        int playerX, playerY;
        int turnCount, savedLevel;

        file >> health >> maxHealth >> shield >> maxShield;
        file >> mana >> maxMana >> points >> level >> exp >> expToNext;
        file >> turnCount >> savedLevel >> combatMode >> playerX >> playerY;

        file >> marker;
        if (marker != "END_PLAYER_DATA") {
            throw LoadException("player data", "end marker not found: " + marker);
        }

        if (file.fail()) {
            throw LoadException("player data", "read operation failed");
        }
        
        if (!validatePlayerData(health, maxHealth, shield, maxShield, mana, maxMana, points, level)) {
            return false;
        }

        player.setHealthDirectly(health);
        player.setMaxHealthDirectly(maxHealth);
        player.setManaDirectly(mana);
        player.setMaxManaDirectly(maxMana);
        player.setShieldDirectly(shield);
        player.setMaxShieldDirectly(maxShield);
        player.setPointsDirectly(points);
        player.setLevelDirectly(level, exp, expToNext);
        
        gameController.setTurnCount(turnCount);
        gameController.setCurrentLevel(savedLevel);

        CombatMode savedMode = (combatMode == 0) ? CombatMode::MELEE : CombatMode::RANGED;
        player.setCombatMode(savedMode);

        if (field.isValidPosition(playerX, playerY)) {
            Position oldPos = gameController.getPlayerPosition();
            if (field.isValidPosition(oldPos.x, oldPos.y)) {
                field.getCell(oldPos.x, oldPos.y).setHasWanderer(false);
            }
            
            gameController.setPlayerPosition(Position(playerX, playerY));
            field.getCell(playerX, playerY).setHasWanderer(true);
            
            std::cout << "Player position set to: (" << playerX << ", " << playerY << ")\n";
        } else {
            throw ValidationException("player position", 
                "(" + std::to_string(playerX) + ", " + std::to_string(playerY) + ")");
        }

        std::cout << "Player data loaded successfully\n";
        return true;
        
    } catch (const ValidationException& e) {
        throw LoadException("player stats", e.what());
    } catch (const std::exception& e) {
        throw LoadException("player stats", std::string("unexpected: ") + e.what());
    }
}

bool PlayerLoader::validatePlayerData(int health, int maxHealth, int shield, int maxShield,
                                     int mana, int maxMana, int points, int level) const {
    if (health < 0 || health > maxHealth) {
        throw ValidationException("player health", 
            std::to_string(health) + "/" + std::to_string(maxHealth));
    }
    if (maxHealth <= 0) {
        throw ValidationException("player max health", std::to_string(maxHealth));
    }
    if (shield < 0 || shield > maxShield) {
        throw ValidationException("player shield", 
            std::to_string(shield) + "/" + std::to_string(maxShield));
    }
    if (mana < 0 || mana > maxMana) {
        throw ValidationException("player mana", 
            std::to_string(mana) + "/" + std::to_string(maxMana));
    }
    if (points < 0) {
        throw ValidationException("player points", std::to_string(points));
    }
    if (level <= 0) {
        throw ValidationException("player level", std::to_string(level));
    }
    return true;
}