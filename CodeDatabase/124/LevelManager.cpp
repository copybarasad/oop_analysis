#include "LevelManager.h"
#include <Windows.h>

LevelManager::LevelManager() : currentLevel(1), totalLevels(3) {}

void LevelManager::setLevel(int level) {
    currentLevel = level;
}

int LevelManager::getCurrentLevel() const {
    return currentLevel;
}

int LevelManager::getTotalLevels() const {
    return totalLevels;
}

void LevelManager::initializeLevel(GameField& field, Player& player, Enemy& enemy, EnemyManager& enemyManager) {
    int fieldSize = 12 + currentLevel * 3;
    field.reset(fieldSize, fieldSize); 
    int obstacleCount = (field.getWidth() * field.getHeight()) / 10;
    
    field.addObstacles(obstacleCount);
    int centerX = field.getWidth() / 2;
    int centerY = field.getHeight() / 2;
    
    if (field.placePlayer(centerX, centerY)) {
    } else {
        bool found = false;
        for (int y = 0; y < field.getHeight() && !found; y++) {
            for (int x = 0; x < field.getWidth() && !found; x++) {
                if (field.placePlayer(x, y)) {
                    found = true;
                }
            }
        }
    }
     
    enemy = Enemy(currentLevel);
    int enemyX = field.getWidth() - 2;
    int enemyY = field.getHeight() - 2;
    
    if (!field.placeEnemy(enemyX, enemyY)) {
        bool enemyFound = false;
        for (int y = field.getHeight() - 1; y >= 0 && !enemyFound; y--) {
            for (int x = field.getWidth() - 1; x >= 0 && !enemyFound; x--) {
                if (field.isCellEmpty(x, y) && field.placeEnemy(x, y)) {
                    enemyFound = true;
                }
            }
        }
    }
     
    enemyManager.placeTowerOnEmptyCell(field);
}

void LevelManager::levelUp(Player& player, EnemyManager& enemyManager) {   
    int nextLevel = currentLevel + 1;
    currentLevel = nextLevel;
    
    showLevelCompleteMessage(player, enemyManager);  
    
    std::cout << "Переход на уровень " << nextLevel << " через 3 секунды...\n";
    Sleep(3000);
     
    int currentSpellCount = player.getSpellHand().getSize();
    int spellsToKeep = currentSpellCount / 2;
    
    std::cout << "Переход на уровень " << nextLevel << "!\n";
    std::cout << "Заклинаний было: " << currentSpellCount << ", останется: " << spellsToKeep << "\n";
    
    while (player.getSpellHand().getSize() > spellsToKeep) {
        player.getSpellHand().removeRandomSpell();
    }
     
    player.setHealth(player.getMaxHealth());
    enemyManager.setLevel(nextLevel);  
    enemyManager.resetForNewLevel();
     
    std::cout << "Оставшиеся заклинания:\n";
    const auto& spellNames = player.getSpellHand().getSpellNames();
    for (size_t i = 0; i < spellNames.size(); ++i) {
        std::cout << "   " << (i + 1) << ". " << spellNames[i] << "\n";
    }
}

bool LevelManager::shouldLevelUp(const EnemyManager& enemyManager) const {
    return enemyManager.isLevelComplete() && currentLevel < totalLevels;
}

void LevelManager::showLevelStartMessage(const GameField& field, const EnemyManager& enemyManager) const {
    std::cout << "\n====================================\n";
    std::cout << "          УРОВЕНЬ " << currentLevel << " из " << totalLevels << "\n";
    std::cout << "Размер поля: " << field.getWidth() << "x" << field.getHeight() << "\n";
    std::cout << "Убито врагов: " << enemyManager.getEnemiesDefeated() << "/" << 2 << "\n";
    std::cout << "====================================\n";
}

void LevelManager::showLevelCompleteMessage(const Player& player, const EnemyManager& enemyManager) const {
    std::cout << "\n🎊 УРОВЕНЬ " << currentLevel-1 << " ПРОЙДЕН! 🎊\n";
    std::cout << "Побеждено врагов: " << enemyManager.getEnemiesDefeated() << "\n";
    std::cout << "Финальный счет уровня: " << player.getEconomy().getScore() << "\n";
}