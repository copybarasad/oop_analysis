#include "GameSaver.h"
#include "GameStateSerializer.h"  
#include "GameExceptions.h"
#include "gamefield.h"
#include "player.h"
#include "enemy.h"
#include "EnemyManager.h"
#include "LevelManager.h"
#include "spellcard.h"
#include <fstream>
#include <iostream>

bool GameSaver::saveGame(int slot, const GameField& field, const Player& player, 
                        const Enemy& enemy, const EnemyManager& enemyManager, const LevelManager& levelManager) {
    try {
        GameState state = collectGameState(field, player, enemy, enemyManager, levelManager);
        validateStateBeforeSave(state);
        
        std::string filename = "save_" + std::to_string(slot) + ".json";  
        writeToFile(filename, state);
        
        std::cout << "Игра сохранена в слот " << slot << "!\n";
        return true;
    } catch (const SaveException& e) {
        std::cout << "Ошибка сохранения: " << e.what() << "\n";
        return false;
    }
}

void GameSaver::writeToFile(const std::string& filename, const GameState& state) {
    std::string jsonStr = GameStateSerializer::toString(state);
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SaveException("Не удалось создать файл: " + filename);
    }
    file << jsonStr;
}

GameState GameSaver::collectGameState(const GameField& field, const Player& player, 
                                     const Enemy& enemy, const EnemyManager& enemyManager, const LevelManager& levelManager) {
    GameState state;
     
    state.level = levelManager.getCurrentLevel();
    state.playerHealth = player.getHealth();
    state.playerScore = player.getEconomy().getScore();
    state.playerMana = player.getEconomy().getMana();
    state.enemiesDefeated = enemyManager.getEnemiesDefeated();
    state.enemyHealth = enemy.getHealth();
    state.enemyLevel = enemy.getLevel();
     
    state.fieldWidth = field.getWidth();
    state.fieldHeight = field.getHeight();
    field.findPlayerPosition(state.playerX, state.playerY);
    field.findEnemyPosition(state.enemyX, state.enemyY);
     
    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            const Cell& cell = field.getCell(x, y);
            if (!cell.getIsPassable() && !cell.getHasPlayer() && !cell.getHasEnemy()) {
                state.obstacles.push_back({x, y});
            }
        }
    }
     
    const auto& towers = field.getTowers();
    for (const auto& tower : towers) {
        if (tower.getIsAlive()) {
            int x, y;
            tower.getPosition(x, y);
            state.towers.push_back(std::make_tuple(x, y, tower.getHealth()));
        }
    }
     
    const auto& traps = field.getTraps();
    for (const auto& trap : traps) {
        if (trap.getIsActive()) {
            int x, y;
            trap.getPosition(x, y);
            state.traps.push_back(std::make_tuple(x, y, trap.getDamage(), true));
        }
    }
     
    const auto& spells = player.getSpellHand().getSpells();
    for (const auto& spell : spells) {
        state.spells.push_back(std::make_pair(spell->getName(), spell->getCurrentCooldown()));
    }
    
    return state;
}

void GameSaver::validateStateBeforeSave(const GameState& state) {
    if (state.playerHealth <= 0) {
        throw SaveException("Невозможно сохранить - игрок мертв");
    }
    
}
 
void GameSaver::showSaves() {
    std::cout << "\n=== СОХРАНЕНИЯ ===\n";
    for (int i = 1; i <= 5; i++) {
        std::string filename = "save_" + std::to_string(i) + ".json";  
        std::ifstream file(filename);
        if (file.is_open()) {
            try { 
                std::stringstream buffer;
                buffer << file.rdbuf();
                GameState state = GameStateSerializer::fromString(buffer.str());
                
                std::cout << i << ". Уровень " << state.level 
                         << ", HP: " << state.playerHealth 
                         << ", Очки: " << state.playerScore << "\n";
            } catch (const std::exception& e) {
                std::cout << i << ". Файл поврежден\n";
            }
        } else {
            std::cout << i << ". Пусто\n";
        }
    }
    std::cout << "==================\n";
}