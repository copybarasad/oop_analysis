#include "../include/SaveGameManager.h"
#include "../include/Game.h"
#include "../include/FileGuard.h"
#include "../include/GameExceptions.h"
#include "../include/GameSnapshot.h"
#include "../include/Player.h"
#include "../include/GameWorld.h"
#include "../include/SpellHand.h"
#include "../include/EnhancementBuffer.h"
#include <iostream>
#include <sstream>
#include <sys/stat.h>

GameState::GameState() 
    : currentLevel(1), playerHealth(100), playerScore(0),
      playerMeleeDamage(40), playerRangedDamage(20),
      combatMode(0), fieldSize(15), turnNumber(0) {}

SaveGameManager::SaveGameManager() {}

bool SaveGameManager::hasSaveFile() const {
    struct stat buffer;
    return (stat(SAVE_FILE, &buffer) == 0);
}

void SaveGameManager::deleteSaveFile() {
    if (hasSaveFile()) {
        remove(SAVE_FILE);
    }
}

GameMode SaveGameManager::readGameMode() const {
    if (!hasSaveFile()) {
        return GameMode::CUSTOM;
    }
    
    try {
        FileGuard fileGuard(SAVE_FILE, std::ios::in);
        
        if (!fileGuard.isFileOpen()) {
            return GameMode::CUSTOM;
        }
        
        std::fstream& file = fileGuard.getStream();
        std::string line;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#' || line[0] == '[') continue;
            
            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;
            
            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);
            
            if (key == "GAME_MODE") {
                return static_cast<GameMode>(std::stoi(value));
            }
        }
    } catch (...) {
        return GameMode::CUSTOM;
    }
    
    return GameMode::CUSTOM;
}

int SaveGameManager::readFieldSize() const {
    if (!hasSaveFile()) {
        return 10;
    }
    
    try {
        FileGuard fileGuard(SAVE_FILE, std::ios::in);
        
        if (!fileGuard.isFileOpen()) {
            return 10;
        }
        
        std::fstream& file = fileGuard.getStream();
        std::string line;
        std::string currentSection;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            if (line[0] == '[') {
                currentSection = line;
                continue;
            }
            
            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;
            
            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);
            
            if (currentSection == "[WORLD]" && key == "WIDTH") {
                return std::stoi(value);
            }
        }
    } catch (...) {
        return 10;
    }
    
    return 10;
}

void SaveGameManager::saveGame(const Game& game, int currentLevel) {
    try {
        FileGuard fileGuard(SAVE_FILE, std::ios::out | std::ios::trunc);
        
        if (!fileGuard.isFileOpen()) {
            throw FileWriteException(SAVE_FILE);
        }
        
        std::fstream& file = fileGuard.getStream();
        
        GameSnapshot snapshot = game.exportState();
        
        file << "SAVEGAME_VERSION=" << SAVE_VERSION << "\n";
        file << "TURN=" << snapshot.turnNumber << "\n";
        file << "LEVEL=" << snapshot.currentLevel << "\n";
        file << "GAME_MODE=" << static_cast<int>(snapshot.gameMode) << "\n";
        
        file << "\n[PLAYER]\n";
        file << "HEALTH=" << snapshot.player.health << "\n";
        file << "MAX_HEALTH=" << snapshot.player.maxHealth << "\n";
        file << "MELEE_DAMAGE=" << snapshot.player.meleeDamage << "\n";
        file << "RANGED_DAMAGE=" << snapshot.player.rangedDamage << "\n";
        file << "SCORE=" << snapshot.player.score << "\n";
        file << "COMBAT_MODE=" << static_cast<int>(snapshot.player.combatMode) << "\n";
        file << "IS_SLOWED=" << snapshot.player.isSlowed << "\n";
        file << "POSITION=" << snapshot.player.position.getX() << "," << snapshot.player.position.getY() << "\n";
        
        file << "\n[PROGRESS]\n";
        file << "EXPERIENCE=" << snapshot.progress.experiencePoints << "\n";
        file << "LEVEL=" << snapshot.progress.level << "\n";
        file << "TOTAL_KILLS=" << snapshot.progress.totalEnemiesKilled << "\n";
        file << "TOTAL_DAMAGE=" << snapshot.progress.totalDamageDealt << "\n";
        
        file << "\n[ENHANCEMENTS]\n";
        file << "FIREBALL=" << snapshot.enhancements.fireballEnhancements << "\n";
        file << "METEOR=" << snapshot.enhancements.meteorEnhancements << "\n";
        file << "TRAP=" << snapshot.enhancements.trapEnhancements << "\n";
        file << "SUMMON=" << snapshot.enhancements.summonEnhancements << "\n";
        
        file << "\n[SPELLS]\n";
        file << "COUNT=" << snapshot.spells.size() << "\n";
        for (size_t i = 0; i < snapshot.spells.size(); i++) {
            const auto& spell = snapshot.spells[i];
            file << "SPELL_" << i << "_TYPE=" << static_cast<int>(spell.type) << "\n";
            file << "SPELL_" << i << "_DAMAGE=" << spell.baseDamage << "\n";
            file << "SPELL_" << i << "_RANGE=" << spell.baseRange << "\n";
            file << "SPELL_" << i << "_AREA=" << spell.baseAreaSize << "\n";
            file << "SPELL_" << i << "_SUMMON=" << spell.baseSummonCount << "\n";
        }
        
        file << "\n[WORLD]\n";
        file << "WIDTH=" << snapshot.world.width << "\n";
        file << "HEIGHT=" << snapshot.world.height << "\n";
        
        file << "GRID=";
        for (int y = 0; y < snapshot.world.height; y++) {
            for (int x = 0; x < snapshot.world.width; x++) {
                file << static_cast<int>(snapshot.world.grid[y][x]);
                if (x < snapshot.world.width - 1 || y < snapshot.world.height - 1) {
                    file << ",";
                }
            }
        }
        file << "\n";
        
        file << "\n[ENEMIES]\n";
        file << "COUNT=" << snapshot.world.enemies.size() << "\n";
        for (size_t i = 0; i < snapshot.world.enemies.size(); i++) {
            const auto& enemy = snapshot.world.enemies[i];
            file << "ENEMY_" << i << "=" << enemy.position.getX() << "," 
                 << enemy.position.getY() << "," << enemy.health << "," << enemy.damage << "\n";
        }
        
        file << "\n[BUILDINGS]\n";
        file << "COUNT=" << snapshot.world.buildings.size() << "\n";
        for (size_t i = 0; i < snapshot.world.buildings.size(); i++) {
            const auto& building = snapshot.world.buildings[i];
            file << "BUILDING_" << i << "=" << building.position.getX() << "," 
                 << building.position.getY() << "," << building.health << "," 
                 << building.turnsUntilSpawn << "\n";
        }
        
        file << "\n[TOWERS]\n";
        file << "COUNT=" << snapshot.world.towers.size() << "\n";
        for (size_t i = 0; i < snapshot.world.towers.size(); i++) {
            const auto& tower = snapshot.world.towers[i];
            file << "TOWER_" << i << "=" << tower.position.getX() << "," 
                 << tower.position.getY() << "," << tower.health << "," 
                 << tower.cooldownRemaining << "\n";
        }
        
        file << "\n[ALLIES]\n";
        file << "COUNT=" << snapshot.world.allies.size() << "\n";
        for (size_t i = 0; i < snapshot.world.allies.size(); i++) {
            const auto& ally = snapshot.world.allies[i];
            file << "ALLY_" << i << "=" << ally.position.getX() << "," 
                 << ally.position.getY() << "," << ally.health << "," << ally.damage << "\n";
        }
        
        file << "\n[TRAPS]\n";
        file << "COUNT=" << snapshot.world.traps.size() << "\n";
        for (size_t i = 0; i < snapshot.world.traps.size(); i++) {
            const auto& trap = snapshot.world.traps[i];
            file << "TRAP_" << i << "=" << trap.position.getX() << "," 
                 << trap.position.getY() << "," << trap.damage << "\n";
        }
        
        file << "\n[END]\n";
        
        if (file.fail()) {
            throw FileWriteException(SAVE_FILE);
        }
        
        std::cout << "Game saved successfully to " << SAVE_FILE << "\n";
        
    } catch (const std::ios_base::failure& e) {
        throw FileWriteException(std::string(SAVE_FILE) + ": " + e.what());
    } catch (const GameException&) {
        throw;
    } catch (const std::exception& e) {
        throw SaveGameException(std::string("Unexpected error: ") + e.what(), "SaveGameManager::saveGame");
    }
}

bool SaveGameManager::loadGame(Game& game, int& outLevel) {
    if (!hasSaveFile()) {
        throw FileNotFoundException(SAVE_FILE);
    }
    
    try {
        FileGuard fileGuard(SAVE_FILE, std::ios::in);
        
        if (!fileGuard.isFileOpen()) {
            throw FileReadException(SAVE_FILE);
        }
        
        std::fstream& file = fileGuard.getStream();
        GameSnapshot snapshot;
        
        std::string line;
        std::string currentSection;
        
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            if (line[0] == '[') {
                currentSection = line;
                continue;
            }
            
            size_t eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;
            
            std::string key = line.substr(0, eqPos);
            std::string value = line.substr(eqPos + 1);
            
            if (key == "SAVEGAME_VERSION") {
                int version = std::stoi(value);
                if (version != SAVE_VERSION) {
                    throw CorruptedDataException("Incompatible save version");
                }
            } else if (key == "TURN") {
                snapshot.turnNumber = std::stoi(value);
            } else if (key == "LEVEL") {
                snapshot.currentLevel = std::stoi(value);
                outLevel = snapshot.currentLevel;
            } else if (key == "GAME_MODE") {
                snapshot.gameMode = static_cast<GameMode>(std::stoi(value));
            } else if (currentSection == "[PLAYER]") {
                if (key == "HEALTH") snapshot.player.health = std::stoi(value);
                else if (key == "MAX_HEALTH") snapshot.player.maxHealth = std::stoi(value);
                else if (key == "MELEE_DAMAGE") snapshot.player.meleeDamage = std::stoi(value);
                else if (key == "RANGED_DAMAGE") snapshot.player.rangedDamage = std::stoi(value);
                else if (key == "SCORE") snapshot.player.score = std::stoi(value);
                else if (key == "COMBAT_MODE") snapshot.player.combatMode = static_cast<CombatMode>(std::stoi(value));
                else if (key == "IS_SLOWED") snapshot.player.isSlowed = (value == "1");
                else if (key == "POSITION") {
                    size_t commaPos = value.find(',');
                    int x = std::stoi(value.substr(0, commaPos));
                    int y = std::stoi(value.substr(commaPos + 1));
                    snapshot.player.position = Position(x, y);
                }
            } else if (currentSection == "[PROGRESS]") {
                if (key == "EXPERIENCE") snapshot.progress.experiencePoints = std::stoi(value);
                else if (key == "LEVEL") snapshot.progress.level = std::stoi(value);
                else if (key == "TOTAL_KILLS") snapshot.progress.totalEnemiesKilled = std::stoi(value);
                else if (key == "TOTAL_DAMAGE") snapshot.progress.totalDamageDealt = std::stoi(value);
            } else if (currentSection == "[ENHANCEMENTS]") {
                if (key == "FIREBALL") snapshot.enhancements.fireballEnhancements = std::stoi(value);
                else if (key == "METEOR") snapshot.enhancements.meteorEnhancements = std::stoi(value);
                else if (key == "TRAP") snapshot.enhancements.trapEnhancements = std::stoi(value);
                else if (key == "SUMMON") snapshot.enhancements.summonEnhancements = std::stoi(value);
            } else if (currentSection == "[SPELLS]") {
                if (key == "COUNT") {
                    snapshot.spells.resize(std::stoi(value));
                } else if (key.find("SPELL_") == 0) {
                    size_t underscorePos = key.find('_', 6);
                    int index = std::stoi(key.substr(6, underscorePos - 6));
                    std::string attribute = key.substr(underscorePos + 1);
                    
                    if (attribute == "TYPE") snapshot.spells[index].type = static_cast<SpellType>(std::stoi(value));
                    else if (attribute == "DAMAGE") snapshot.spells[index].baseDamage = std::stoi(value);
                    else if (attribute == "RANGE") snapshot.spells[index].baseRange = std::stoi(value);
                    else if (attribute == "AREA") snapshot.spells[index].baseAreaSize = std::stoi(value);
                    else if (attribute == "SUMMON") snapshot.spells[index].baseSummonCount = std::stoi(value);
                }
            } else if (currentSection == "[WORLD]") {
                if (key == "WIDTH") snapshot.world.width = std::stoi(value);
                else if (key == "HEIGHT") snapshot.world.height = std::stoi(value);
                else if (key == "GRID") {
                    snapshot.world.grid.resize(snapshot.world.height, 
                                               std::vector<CellType>(snapshot.world.width));
                    
                    std::istringstream ss(value);
                    std::string cell;
                    int y = 0, x = 0;
                    
                    while (std::getline(ss, cell, ',')) {
                        snapshot.world.grid[y][x] = static_cast<CellType>(std::stoi(cell));
                        x++;
                        if (x >= snapshot.world.width) {
                            x = 0;
                            y++;
                        }
                    }
                }
            } else if (currentSection == "[ENEMIES]") {
                if (key == "COUNT") {
                    snapshot.world.enemies.resize(std::stoi(value));
                } else if (key.find("ENEMY_") == 0) {
                    int index = std::stoi(key.substr(6));
                    std::istringstream ss(value);
                    std::string part;
                    int x, y, health, damage;
                    
                    std::getline(ss, part, ','); x = std::stoi(part);
                    std::getline(ss, part, ','); y = std::stoi(part);
                    std::getline(ss, part, ','); health = std::stoi(part);
                    std::getline(ss, part, ','); damage = std::stoi(part);
                    
                    snapshot.world.enemies[index].position = Position(x, y);
                    snapshot.world.enemies[index].health = health;
                    snapshot.world.enemies[index].damage = damage;
                }
            } else if (currentSection == "[BUILDINGS]") {
                if (key == "COUNT") {
                    snapshot.world.buildings.resize(std::stoi(value));
                } else if (key.find("BUILDING_") == 0) {
                    int index = std::stoi(key.substr(9));
                    std::istringstream ss(value);
                    std::string part;
                    int x, y, health, turns;
                    
                    std::getline(ss, part, ','); x = std::stoi(part);
                    std::getline(ss, part, ','); y = std::stoi(part);
                    std::getline(ss, part, ','); health = std::stoi(part);
                    std::getline(ss, part, ','); turns = std::stoi(part);
                    
                    snapshot.world.buildings[index].position = Position(x, y);
                    snapshot.world.buildings[index].health = health;
                    snapshot.world.buildings[index].turnsUntilSpawn = turns;
                }
            } else if (currentSection == "[TOWERS]") {
                if (key == "COUNT") {
                    snapshot.world.towers.resize(std::stoi(value));
                } else if (key.find("TOWER_") == 0) {
                    int index = std::stoi(key.substr(6));
                    std::istringstream ss(value);
                    std::string part;
                    int x, y, health, cooldown;
                    
                    std::getline(ss, part, ','); x = std::stoi(part);
                    std::getline(ss, part, ','); y = std::stoi(part);
                    std::getline(ss, part, ','); health = std::stoi(part);
                    std::getline(ss, part, ','); cooldown = std::stoi(part);
                    
                    snapshot.world.towers[index].position = Position(x, y);
                    snapshot.world.towers[index].health = health;
                    snapshot.world.towers[index].cooldownRemaining = cooldown;
                }
            } else if (currentSection == "[ALLIES]") {
                if (key == "COUNT") {
                    snapshot.world.allies.resize(std::stoi(value));
                } else if (key.find("ALLY_") == 0) {
                    int index = std::stoi(key.substr(5));
                    std::istringstream ss(value);
                    std::string part;
                    int x, y, health, damage;
                    
                    std::getline(ss, part, ','); x = std::stoi(part);
                    std::getline(ss, part, ','); y = std::stoi(part);
                    std::getline(ss, part, ','); health = std::stoi(part);
                    std::getline(ss, part, ','); damage = std::stoi(part);
                    
                    snapshot.world.allies[index].position = Position(x, y);
                    snapshot.world.allies[index].health = health;
                    snapshot.world.allies[index].damage = damage;
                }
            } else if (currentSection == "[TRAPS]") {
                if (key == "COUNT") {
                    snapshot.world.traps.resize(std::stoi(value));
                } else if (key.find("TRAP_") == 0) {
                    int index = std::stoi(key.substr(5));
                    std::istringstream ss(value);
                    std::string part;
                    int x, y, damage;
                    
                    std::getline(ss, part, ','); x = std::stoi(part);
                    std::getline(ss, part, ','); y = std::stoi(part);
                    std::getline(ss, part, ','); damage = std::stoi(part);
                    
                    snapshot.world.traps[index].position = Position(x, y);
                    snapshot.world.traps[index].damage = damage;
                }
            }
        }
        
        game.importState(snapshot);
        
        std::cout << "Game loaded successfully from " << SAVE_FILE << "\n";
        return true;
        
    } catch (const std::invalid_argument& e) {
        throw CorruptedDataException("Invalid number format in save file");
    } catch (const std::out_of_range& e) {
        throw CorruptedDataException("Number out of range in save file");
    } catch (const std::ios_base::failure& e) {
        throw FileReadException(std::string(SAVE_FILE) + ": " + e.what());
    } catch (const GameException&) {
        throw;
    } catch (const std::exception& e) {
        throw LoadGameException(std::string("Unexpected error: ") + e.what(), "SaveGameManager::loadGame");
    }
}
