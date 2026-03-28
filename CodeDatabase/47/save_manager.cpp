#include "save_manager.h"
#include "game_exceptions.h"
#include "level_manager.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h" 
#include "trap_spell.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

const std::string SaveManager::SAVE_FILE_PATH = "game_save.dat";

bool SaveManager::saveGame(const GameState& gameState, const std::string& filename) {
    std::string filePath = getSaveFilePath(filename);
    
    try {
        std::ofstream file(filePath, std::ios::binary);
        if (!file) {
            throw SaveException("Cannot open file for writing: " + filePath);
        }
        
        int level = gameState.getCurrentLevel();
        int score = gameState.getTotalScore();
        int enemiesDefeated = gameState.getEnemiesDefeated();
        
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
        file.write(reinterpret_cast<const char*>(&score), sizeof(score));
        file.write(reinterpret_cast<const char*>(&enemiesDefeated), sizeof(enemiesDefeated));
        
        if (!file) {
            throw SaveException("Failed to write basic game state");
        }
        
        // Save player state with all upgrades
        const Player& player = gameState.getPlayer();
        int health = player.getHealth();
        int maxHealth = player.getMaxHealth();
        int meleeDamage = player.getMeleeDamage();
        int rangedDamage = player.getRangedDamage();
        int playerX = player.getPositionX();
        int playerY = player.getPositionY();
        int combatMode = static_cast<int>(player.getCombatMode());
        int currentMana = player.getSpellHand().getCurrentMana();
        int maxMana = player.getSpellHand().getMaxMana();
        
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
        file.write(reinterpret_cast<const char*>(&meleeDamage), sizeof(meleeDamage));
        file.write(reinterpret_cast<const char*>(&rangedDamage), sizeof(rangedDamage));
        file.write(reinterpret_cast<const char*>(&playerX), sizeof(playerX));
        file.write(reinterpret_cast<const char*>(&playerY), sizeof(playerY));
        file.write(reinterpret_cast<const char*>(&combatMode), sizeof(combatMode));
        file.write(reinterpret_cast<const char*>(&currentMana), sizeof(currentMana));
        file.write(reinterpret_cast<const char*>(&maxMana), sizeof(maxMana));
        
        if (!file) {
            throw SaveException("Failed to write player data");
        }
        
        const auto& spells = player.getSpellHand().getSpells();
        int spellCount = spells.size();
        file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
        
        for (const auto& spell : spells) {
            int spellType = 0;
            std::string spellName = spell->getName();
            
            if (spellName == "Fire discharge") spellType = 0;
            else if (spellName == "Fireball") spellType = 1;
            else if (spellName == "Bear Trap") spellType = 2;
            
            file.write(reinterpret_cast<const char*>(&spellType), sizeof(spellType));
            
            int damage = 0;
            int range = 0;
            
            if (spellType == 0) {
                auto directSpell = dynamic_cast<DirectDamageSpell*>(spell.get());
                if (directSpell) {
                    damage = directSpell->getDamage();
                    range = directSpell->getRange();
                }
            } else if (spellType == 1) {
                auto* areaSpell = dynamic_cast<AreaDamageSpell*>(spell.get());
                if (areaSpell) {
                    damage = areaSpell->getDamage();
                    range = areaSpell->getRange();
                }
            } else if (spellType == 2) {
                auto* trapSpell = dynamic_cast<TrapSpell*>(spell.get());
                if (trapSpell) {
                    damage = trapSpell->getDamage();
                    range = trapSpell->getRange();
                }
            }
            
            file.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
            file.write(reinterpret_cast<const char*>(&range), sizeof(range));
        }

        if (!file) {
            throw SaveException("Failed to write spell data");
        }

        const GameField& field = gameState.getField();
        int fieldWidth = field.getWidth();
        int fieldHeight = field.getHeight();
        file.write(reinterpret_cast<const char*>(&fieldWidth), sizeof(fieldWidth));
        file.write(reinterpret_cast<const char*>(&fieldHeight), sizeof(fieldHeight));
        
        for (int y = 0; y < fieldHeight; ++y) {
            for (int x = 0; x < fieldWidth; ++x) {
                CellType cellType = field.getCellType(x, y);
                file.write(reinterpret_cast<const char*>(&cellType), sizeof(cellType));
            }
        }
        
        if (!file) {
            throw SaveException("Failed to write field data");
        }
        
        const auto& enemies = gameState.getEnemies();
        int enemyCount = enemies.size();
        file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
        
        for (const auto& enemy : enemies) {
            int enemyX = enemy.getPositionX();
            int enemyY = enemy.getPositionY();
            int enemyHealth = enemy.getHealth();
            file.write(reinterpret_cast<const char*>(&enemyX), sizeof(enemyX));
            file.write(reinterpret_cast<const char*>(&enemyY), sizeof(enemyY));
            file.write(reinterpret_cast<const char*>(&enemyHealth), sizeof(enemyHealth));
        }
        
        if (!file) {
            throw SaveException("Failed to write enemy data");
        }
        
        const EnemyBuilding& building = gameState.getEnemyBuilding();
        int buildingX = building.getPositionX();
        int buildingY = building.getPositionY();
        int buildingHealth = building.getHealth();
        int buildingCooldown = building.getCurrentCooldown();
        file.write(reinterpret_cast<const char*>(&buildingX), sizeof(buildingX));
        file.write(reinterpret_cast<const char*>(&buildingY), sizeof(buildingY));
        file.write(reinterpret_cast<const char*>(&buildingHealth), sizeof(buildingHealth));
        file.write(reinterpret_cast<const char*>(&buildingCooldown), sizeof(buildingCooldown));
        
        if (!file) {
            throw SaveException("Failed to write building data");
        }
        
        const EnemyTower& tower = gameState.getEnemyTower();
        int towerX = tower.getPositionX();
        int towerY = tower.getPositionY();
        int towerHealth = tower.getHealth();
        int towerCooldown = tower.getCurrentCooldown();
        file.write(reinterpret_cast<const char*>(&towerX), sizeof(towerX));
        file.write(reinterpret_cast<const char*>(&towerY), sizeof(towerY));
        file.write(reinterpret_cast<const char*>(&towerHealth), sizeof(towerHealth));
        file.write(reinterpret_cast<const char*>(&towerCooldown), sizeof(towerCooldown));
        
        if (!file) {
            throw SaveException("Failed to write tower data");
        }
        
        const auto& traps = gameState.getTraps();
        int trapCount = traps.size();
        file.write(reinterpret_cast<const char*>(&trapCount), sizeof(trapCount));
        
        for (const auto& trap : traps) {
            int trapX = trap.getPositionX();
            int trapY = trap.getPositionY();
            int trapDamage = trap.getDamage();
            bool trapActive = trap.isActive();
            file.write(reinterpret_cast<const char*>(&trapX), sizeof(trapX));
            file.write(reinterpret_cast<const char*>(&trapY), sizeof(trapY));
            file.write(reinterpret_cast<const char*>(&trapDamage), sizeof(trapDamage));
            file.write(reinterpret_cast<const char*>(&trapActive), sizeof(trapActive));
        }
        
        if (!file) {
            throw SaveException("Failed to write trap data or final write error");
        }
        
        return true;
        
    }
    catch (const std::exception& e) {
        throw SaveException(std::string("Failed to save game: ") + e.what());
    }
}

std::unique_ptr<GameState> SaveManager::loadGame(const std::string& filename) {
    std::string filePath = getSaveFilePath(filename);
    
    try {
        if (!saveExists(filename)) {
            throw FileNotFoundException(filePath);
        }
        
        // RAII: файл автоматически закроется при выходе из scope
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            throw LoadException("Cannot open file for reading: " + filePath);
        }
        
        // === 1. ЧТЕНИЕ БАЗОВОГО СОСТОЯНИЯ ===
        int level, score, enemiesDefeated;
        file.read(reinterpret_cast<char*>(&level), sizeof(level));
        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        file.read(reinterpret_cast<char*>(&enemiesDefeated), sizeof(enemiesDefeated));
        
        if (!file) {
            throw CorruptedSaveException("Invalid file format - basic game state");
        }
        
        // === 2. ЧТЕНИЕ СОСТОЯНИЯ ИГРОКА ===
        int health, maxHealth, meleeDamage, rangedDamage, playerX, playerY, combatMode, currentMana, maxMana;
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
        file.read(reinterpret_cast<char*>(&meleeDamage), sizeof(meleeDamage));
        file.read(reinterpret_cast<char*>(&rangedDamage), sizeof(rangedDamage));
        file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
        file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
        file.read(reinterpret_cast<char*>(&combatMode), sizeof(combatMode));
        file.read(reinterpret_cast<char*>(&currentMana), sizeof(currentMana));
        file.read(reinterpret_cast<char*>(&maxMana), sizeof(maxMana));
        
        if (!file) {
            throw CorruptedSaveException("Invalid player data");
        }
        
        // === 3. ЧТЕНИЕ ЗАКЛИНАНИЙ ===
        int spellCount;
        file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
        
        std::vector<std::unique_ptr<Spell>> loadedSpells;
        if (spellCount > 0) {
            for (int i = 0; i < spellCount; ++i) {
                int spellType, damage, range;
                file.read(reinterpret_cast<char*>(&spellType), sizeof(spellType));
                file.read(reinterpret_cast<char*>(&damage), sizeof(damage));
                file.read(reinterpret_cast<char*>(&range), sizeof(range));
                
                if (!file) {
                    throw CorruptedSaveException("Invalid spell data at index " + std::to_string(i));
                }
                
                std::unique_ptr<Spell> spell;
                switch (spellType) {
                    case 0:
                        spell = std::make_unique<DirectDamageSpell>(damage, range);
                        break;
                    case 1:
                        spell = std::make_unique<AreaDamageSpell>(damage, range);
                        break;
                    case 2:
                        spell = std::make_unique<TrapSpell>(damage, range);
                        break;
                    default:
                        throw CorruptedSaveException("Unknown spell type: " + std::to_string(spellType));
                }
                loadedSpells.push_back(std::move(spell));
            }
        }
        
        // === 4. ЧТЕНИЕ РАЗМЕРОВ ПОЛЯ ===
        int fieldWidth, fieldHeight;
        file.read(reinterpret_cast<char*>(&fieldWidth), sizeof(fieldWidth));
        file.read(reinterpret_cast<char*>(&fieldHeight), sizeof(fieldHeight));
        
        if (!file) {
            throw CorruptedSaveException("Invalid field dimensions");
        }
        
        // === 5. СОЗДАНИЕ СОСТОЯНИЯ ИГРЫ ===
        auto gameState = std::make_unique<GameState>(fieldWidth, fieldHeight, level);
        
        // === 6. ВОССТАНОВЛЕНИЕ СОСТОЯНИЯ ИГРОКА ===
        Player& player = gameState->getPlayer();
        
        // Восстановление характеристик
        if (maxHealth > player.getMaxHealth()) {
            player.increaseMaxHealth(maxHealth - player.getMaxHealth());
        }
        player.setHealth(health);
        
        if (meleeDamage > player.getMeleeDamage()) {
            player.increaseMeleeDamage(meleeDamage - player.getMeleeDamage());
        }
        if (rangedDamage > player.getRangedDamage()) {
            player.increaseRangedDamage(rangedDamage - player.getRangedDamage());
        }
        
        // Восстановление позиции
        player.move(playerX - player.getPositionX(), playerY - player.getPositionY());
        
        // Восстановление режима боя
        if (static_cast<CombatMode>(combatMode) != player.getCombatMode()) {
            player.switchCombatMode();
        }
        
        // Восстановление маны
        if (maxMana > player.getSpellHand().getMaxMana()) {
            player.increaseManaCapacity(maxMana - player.getSpellHand().getMaxMana());
        }
        player.getSpellHand().setCurrentMana(currentMana);
        
        // Восстановление заклинаний
        player.getSpellHand().clearSpells();
        for (auto& spell : loadedSpells) {
            if (!player.getSpellHand().addSpell(std::move(spell))) {
                std::cout << "Warning: Could not add spell to hand" << std::endl;
            }
        }
        
        gameState->setTotalScore(score);
        gameState->setEnemiesDefeated(enemiesDefeated);
        
        // === 7. ВОССТАНОВЛЕНИЕ СОСТОЯНИЯ ПОЛЯ ===
        GameField& field = gameState->getField();
        for (int y = 0; y < fieldHeight; ++y) {
            for (int x = 0; x < fieldWidth; ++x) {
                CellType cellType;
                file.read(reinterpret_cast<char*>(&cellType), sizeof(cellType));
                if (!file) {
                    throw CorruptedSaveException("Invalid field cell data at (" + 
                                                std::to_string(x) + ", " + std::to_string(y) + ")");
                }
                field.setCellType(x, y, cellType);
            }
        }
        
        // === 8. ВОССТАНОВЛЕНИЕ ВРАГОВ ===
        int enemyCount;
        file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
        
        if (enemyCount > 0) {
            auto& enemies = gameState->getEnemies();
            enemies.clear();
            
            for (int i = 0; i < enemyCount; ++i) {
                int enemyX, enemyY, enemyHealth;
                file.read(reinterpret_cast<char*>(&enemyX), sizeof(enemyX));
                file.read(reinterpret_cast<char*>(&enemyY), sizeof(enemyY));
                file.read(reinterpret_cast<char*>(&enemyHealth), sizeof(enemyHealth));
                
                if (!file) {
                    throw CorruptedSaveException("Invalid enemy data at index " + std::to_string(i));
                }
                
                Enemy enemy(enemyX, enemyY);
                enemy.setHealth(enemyHealth);
                enemies.push_back(enemy);
            }
        }
        
        // === 9. ВОССТАНОВЛЕНИЕ ВРАЖЕСКОГО ЗДАНИЯ ===
        int buildingX, buildingY, buildingHealth, buildingCooldown;
        file.read(reinterpret_cast<char*>(&buildingX), sizeof(buildingX));
        file.read(reinterpret_cast<char*>(&buildingY), sizeof(buildingY));
        file.read(reinterpret_cast<char*>(&buildingHealth), sizeof(buildingHealth));
        file.read(reinterpret_cast<char*>(&buildingCooldown), sizeof(buildingCooldown));
        
        if (!file) {
            throw CorruptedSaveException("Invalid building data");
        }
        
        EnemyBuilding& building = gameState->getEnemyBuilding();
        building.setPosition(buildingX, buildingY);
        building.setHealth(buildingHealth);
        building.setCooldown(buildingCooldown);
        
        // === 10. ВОССТАНОВЛЕНИЕ ВРАЖЕСКОЙ БАШНИ ===
        int towerX, towerY, towerHealth, towerCooldown;
        file.read(reinterpret_cast<char*>(&towerX), sizeof(towerX));
        file.read(reinterpret_cast<char*>(&towerY), sizeof(towerY));
        file.read(reinterpret_cast<char*>(&towerHealth), sizeof(towerHealth));
        file.read(reinterpret_cast<char*>(&towerCooldown), sizeof(towerCooldown));
        
        if (!file) {
            throw CorruptedSaveException("Invalid tower data");
        }
        
        EnemyTower& tower = gameState->getEnemyTower();
        tower.setPosition(towerX, towerY);
        tower.setHealth(towerHealth);
        tower.setCooldown(towerCooldown);
        
        // === 11. ВОССТАНОВЛЕНИЕ ЛОВУШЕК ===
        int trapCount;
        file.read(reinterpret_cast<char*>(&trapCount), sizeof(trapCount));
        
        if (trapCount > 0) {
            auto& traps = gameState->getTraps();
            traps.clear();
            
            for (int i = 0; i < trapCount; ++i) {
                int trapX, trapY, trapDamage;
                bool trapActive;
                file.read(reinterpret_cast<char*>(&trapX), sizeof(trapX));
                file.read(reinterpret_cast<char*>(&trapY), sizeof(trapY));
                file.read(reinterpret_cast<char*>(&trapDamage), sizeof(trapDamage));
                file.read(reinterpret_cast<char*>(&trapActive), sizeof(trapActive));
                
                if (!file) {
                    throw CorruptedSaveException("Invalid trap data at index " + std::to_string(i));
                }
                
                Trap trap(trapX, trapY, trapDamage);
                if (!trapActive) {
                    trap.trigger();
                }
                traps.push_back(trap);
            }
        }

        // Финальная проверка - убеждаемся, что не было ошибок чтения
        if (!file) {
            throw CorruptedSaveException("Final read error - file may be truncated");
        }
        
        // Файл автоматически закроется здесь благодаря RAII
        
        std::cout << "Game loaded successfully from " << filePath << std::endl;
        std::cout << "Level: " << level << ", Score: " << score << std::endl;
        std::cout << "Player: " << player.getHealth() << "/" << player.getMaxHealth() << " HP" << std::endl;
        std::cout << "Field: " << fieldWidth << "x" << fieldHeight << std::endl;
        std::cout << "Enemies: " << enemyCount << ", Traps: " << trapCount << std::endl;
        std::cout << "Spells: " << player.getSpellHand().getSpellCount() << std::endl;
        
        return gameState;
        
    } catch (const std::exception& e) {
        throw LoadException(std::string("Failed to load game: ") + e.what());
    }
}

bool SaveManager::saveExists(const std::string& filename) {
    std::string filePath = getSaveFilePath(filename);
    std::ifstream file(filePath);
    return file.good();
}

std::string SaveManager::getSaveFilePath(const std::string& filename) {
    return filename;
}