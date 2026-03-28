#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "IInputAdapter.h"
#include "IRenderAdapter.h"
#include "GameVisualizer.h"
#include "GameStateManager.h"
#include "gamefield.h"
#include "player.h"
#include "enemy.h"
#include "EnemyManager.h"
#include "LevelManager.h"
#include <memory>
#include <iostream>
#include <functional>

template<typename InputAdapter, typename RenderAdapter>
class GameController {
private: 
    GameField& field;
    Player& player;
    Enemy& enemy;
    EnemyManager& enemyManager;
    LevelManager& levelManager;
    GameStateManager& stateManager;
     
    std::unique_ptr<InputAdapter> inputAdapter;
     
    GameVisualizer<RenderAdapter> visualizer;
    
    bool isLoadingFromSave;
     
    std::function<void()> restartCallback;

public: 
    GameController(GameField& gameField, 
                   Player& gamePlayer, 
                   Enemy& gameEnemy,
                   EnemyManager& manager,
                   LevelManager& lvlManager,
                   GameStateManager& stManager,
                   std::unique_ptr<InputAdapter> input,
                   std::unique_ptr<RenderAdapter> render,
                   std::function<void()> onRestart = nullptr)
        : field(gameField), 
          player(gamePlayer), 
          enemy(gameEnemy),
          enemyManager(manager), 
          levelManager(lvlManager), 
          stateManager(stManager),
          inputAdapter(std::move(input)),
          visualizer(std::move(render)),
          isLoadingFromSave(false),
          restartCallback(onRestart) {
    }
    
    void run();
    void setLoadingFromSave(bool loading) { isLoadingFromSave = loading; }
     
    GameField& getField() { return field; }
    Player& getPlayer() { return player; }
    Enemy& getEnemy() { return enemy; }
    EnemyManager& getEnemyManager() { return enemyManager; }
    LevelManager& getLevelManager() { return levelManager; }
    GameVisualizer<RenderAdapter>& getVisualizer() { return visualizer; }

private:
    void initializeGame();
    void processGameLoop();
    void handleLevelTransition();
};
 
template<typename InputAdapter, typename RenderAdapter>
void GameController<InputAdapter, RenderAdapter>::run() {
    initializeGame();
    processGameLoop();
}

template<typename InputAdapter, typename RenderAdapter>
void GameController<InputAdapter, RenderAdapter>::initializeGame() {
    if (!isLoadingFromSave) {
        enemyManager.setLevel(1);
        enemyManager.resetForNewLevel();
        if (player.getSpellHand().getSize() == 0) {
            player.getSpellHand().addRandomSpell();
        }
        levelManager.initializeLevel(field, player, enemy, enemyManager);
    }
     
    visualizer.renderHelp();
    
    if (isLoadingFromSave) {
        std::cout << "\n====================================\n";
        std::cout << "       ЗАГРУЖЕНА СОХРАНЕННАЯ ИГРА\n";
        std::cout << "          УРОВЕНЬ " << levelManager.getCurrentLevel() << "\n";
        std::cout << "Размер поля: " << field.getWidth() << "x" << field.getHeight() << "\n";
        std::cout << "Убито врагов: " << enemyManager.getEnemiesDefeated() << "/2\n";
        std::cout << "====================================\n";
        isLoadingFromSave = false;
    }
}

template<typename InputAdapter, typename RenderAdapter>
void GameController<InputAdapter, RenderAdapter>::processGameLoop() {
    while (stateManager.isGameRunning()) {
        levelManager.showLevelStartMessage(field, enemyManager);
        stateManager.setLevelCompleted(false);
        
        while (stateManager.isGameRunning() && !stateManager.isLevelCompleted() && player.isAlive()) {
            
            visualizer.renderGame(field, player, enemy, enemyManager);
            player.updateSpellCooldowns();
            
            char input;
            std::cout << "Введите команду (h для справки): ";
            std::cin >> input;
            while (std::cin.get() != '\n') continue;
            
            if (input == 'h' || input == 'H') {
                inputAdapter->printHelp();
                continue;
            }
            
            if (input == 'r' || input == 'R') {
                if (inputAdapter->reloadConfig()) {
                    std::cout << "Конфигурация управления перезагружена!\n";
                } else {
                    std::cout << "Ошибка перезагрузки конфигурации!\n";
                }
                continue;
            }
            
            bool turnConsumed = inputAdapter->handleInput(input, player, field, enemy, enemyManager, levelManager);
            
            if (turnConsumed) {
                enemyManager.updateEnemy(enemy, field, player);
                field.updateTraps(enemy);
                field.updateTowers(player);
                
                if (stateManager.checkLevelComplete(enemyManager)) {
                    stateManager.setLevelCompleted(true);
                } else if (stateManager.checkGameOver(player)) {
                    stateManager.handleGameOver(player, enemyManager, field, levelManager, enemy, restartCallback);
                }
            }
        }
        
        handleLevelTransition();
    }
}

template<typename InputAdapter, typename RenderAdapter>
void GameController<InputAdapter, RenderAdapter>::handleLevelTransition() {
    if (stateManager.isLevelCompleted()) {
        if (levelManager.shouldLevelUp(enemyManager)) {
            levelManager.levelUp(player, enemyManager);
            levelManager.initializeLevel(field, player, enemy, enemyManager);
        } else {
            std::cout << "\n====================================\n";
            std::cout << "       🎉 ПОЗДРАВЛЯЕМ! 🎉\n";
            std::cout << "  ВЫ ПРОШЛИ ВСЕ " << levelManager.getTotalLevels() << " УРОВНЯ!\n";
            std::cout << "====================================\n";
            std::cout << "Финальный счет: " << player.getEconomy().getScore() << " очков\n";
            std::cout << "Всего побеждено врагов: " << enemyManager.getEnemiesDefeated() << "\n";
            std::cout << "Здоровье: " << player.getHealth() << "/" << player.getMaxHealth() << "\n";
            std::cout << "Заклинаний в арсенале: " << player.getSpellHand().getSize() << "\n";
            std::cout << "\nСпасибо за игру! До новых встреч!\n";
            stateManager.setGameRunning(false);
        }
    }
}

#endif