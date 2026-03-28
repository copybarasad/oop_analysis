#include "GameStateManager.h" 
#include <functional>

GameStateManager::GameStateManager() : gameRunning(true), levelCompleted(false) {}

bool GameStateManager::isGameRunning() const {
    return gameRunning;
}

bool GameStateManager::isLevelCompleted() const {
    return levelCompleted;
}

void GameStateManager::setGameRunning(bool running) {
    gameRunning = running;
}

void GameStateManager::setLevelCompleted(bool completed) {
    levelCompleted = completed;
}

bool GameStateManager::checkGameOver(const Player& player) const {
    if (!player.isAlive()) {
        std::cout << "Игра окончена! Вы проиграли!\n"; 
        return true;
    }
    return false;
}

bool GameStateManager::checkLevelComplete(const EnemyManager& enemyManager) const {
    return enemyManager.isLevelComplete();
}

// ИЗМЕНЯЕМ: добавляем параметр restartCallback
void GameStateManager::handleGameOver(Player& player, EnemyManager& enemyManager, 
                                     GameField& field, LevelManager& levelManager, 
                                     Enemy& enemy, std::function<void()> restartCallback) {
    std::cout << "ИГРА ОКОНЧЕНА! Вы проиграли на уровне " << enemyManager.getCurrentLevel() << "!\n";
    std::cout << "Счет: " << player.getEconomy().getScore() 
              << ", Побеждено врагов: " << enemyManager.getEnemiesDefeated() << std::endl;
    
    std::cout << "Хотите начать заново? (y/n): ";
    char choice;
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') { 
        gameRunning = true;
        levelCompleted = false;
        
        // ИСПРАВЛЯЕМ: используем коллбэк вместо создания объектов
        if (restartCallback) {
            restartCallback();
        } else {
            // Если коллбэк не предоставлен, создаем объекты по умолчанию
            // (для обратной совместимости, но в main.cpp мы передадим коллбэк)
            player = Player(3);
            player.getSpellHand().addRandomSpell();
            enemyManager.reset();
            enemyManager.setLevel(1);
            field.reset(15, 15);
            levelManager.setLevel(1);
        }
        
        std::cout << "Игра перезапущена! Начинаем с уровня 1.\n";
    } else {
        gameRunning = false;
    }
}