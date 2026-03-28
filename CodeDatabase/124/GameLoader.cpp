#include "GameLoader.h"
#include "GameStateSerializer.h" 
#include "GameExceptions.h"
#include "gamefield.h"
#include "player.h"
#include "enemy.h"
#include "EnemyManager.h"
#include "LevelManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool GameLoader::loadGame(int slot, GameField& field, Player& player, 
                         Enemy& enemy, EnemyManager& enemyManager, LevelManager& levelManager) {
    try {
        GameState state = readFromFile(slot);
        validateGameState(state);
        restoreGameState(state, field, player, enemy, enemyManager, levelManager);
        
        std::cout << "Игра загружена! Уровень " << state.level << std::endl;
        return true;
        
    } catch (const FileNotFoundException& e) {
        std::cout <<  e.what() << "\n Создайте новую игру\n";
        return false;
    } catch (const CorruptedSaveException& e) {
        std::cout <<  e.what() << "\n Файл поврежден\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "Ошибка: " << e.what() << "\n";
        return false;
    }
}
 
GameState GameLoader::readFromFile(int slot) {
    std::string filename = "save_" + std::to_string(slot) + ".json";  
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }
    
    try { 
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonStr = buffer.str();
         
        return GameStateSerializer::fromString(jsonStr);
        
    } catch (const std::exception& e) {
        throw CorruptedSaveException("Не удалось разобрать JSON: " + std::string(e.what()));
    }
}
 

void GameLoader::validateGameState(const GameState& state) {
    if (state.playerHealth < 0 || state.playerHealth > 100) {
        throw ValidationException("Недостаточное здоровье игрока: " + std::to_string(state.playerHealth));
    }
    if (state.fieldWidth <= 0 || state.fieldHeight <= 0) {
        throw ValidationException("Недопустимый размер поля");
    }
    if (state.level < 1) {
        throw ValidationException("Недопустимый уровень");
    }
     
    if (state.playerX < 0 || state.playerX >= state.fieldWidth ||
        state.playerY < 0 || state.playerY >= state.fieldHeight) {
        throw ValidationException("Недопустимая позиция игрока");
    }
    
    if (state.enemyX < 0 || state.enemyX >= state.fieldWidth ||
        state.enemyY < 0 || state.enemyY >= state.fieldHeight) {
        throw ValidationException("Недопустимая позиция врага");
    }
}

void GameLoader::restoreGameState(const GameState& state, GameField& field, Player& player, 
                                 Enemy& enemy, EnemyManager& enemyManager, LevelManager& levelManager) {
     
    field.reset(state.fieldWidth, state.fieldHeight);
     
    for (const auto& tower : state.towers) {
        int x = std::get<0>(tower);
        int y = std::get<1>(tower);
        int health = std::get<2>(tower);
        
        if (x >= 0 && x < state.fieldWidth && y >= 0 && y < state.fieldHeight) {
            if (field.placeTower(x, y)) {
                auto& towers = field.getTowers();
                if (!towers.empty()) {
                    towers.back().setHealth(health);
                }
            }
        }
    }
    for (const auto& obstacle : state.obstacles) {
        if (obstacle.first >= 0 && obstacle.first < state.fieldWidth &&
            obstacle.second >= 0 && obstacle.second < state.fieldHeight) {
            field.addObstacle(obstacle.first, obstacle.second);
        }
    } 
    for (const auto& trap : state.traps) {
        int x = std::get<0>(trap);
        int y = std::get<1>(trap);
        int damage = std::get<2>(trap);
        
        if (x >= 0 && x < state.fieldWidth && y >= 0 && y < state.fieldHeight) {
            field.placeTrap(x, y, damage);
        }
    }
     
    player.setHealth(state.playerHealth);
    player.getEconomy().setScore(state.playerScore);
    player.getEconomy().setMana(state.playerMana);
     
    enemy = Enemy(state.enemyLevel);
    enemy.setHealth(state.enemyHealth);
     
    if (!field.placePlayer(state.playerX, state.playerY)) {
        std::cout << "Не удалось разместить игрока\n";
         
    }
    
    if (!field.placeEnemy(state.enemyX, state.enemyY)) {
        std::cout << "Не удалось разместить врага\n";
         
    }
     
    levelManager.setLevel(state.level);
    enemyManager.setEnemiesDefeated(state.enemiesDefeated);
    player.getEconomy().setEnemiesDefeated(state.enemiesDefeated);

    player.getSpellHand().clear();
    for (const auto& spellData : state.spells) {
        if (!spellData.first.empty()) {
            player.getSpellHand().addSpellByName(spellData.first); 
        }
    }
}


