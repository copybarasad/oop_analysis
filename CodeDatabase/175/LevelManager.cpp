#include "LevelManager.h"
#include "GameState.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyTower.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <random>

std::unique_ptr<GameState> LevelManager::createLevel(int level, std::shared_ptr<Player> existingPlayer) {
    auto config = getLevelConfig(level);
    
    auto field = std::make_shared<Field>(config.width, config.height);
    field->initializeRandomObstacles(config.wallProbability, config.slowProbability);
    
    std::shared_ptr<Player> player;
    
    if (existingPlayer) {
        player = existingPlayer;
        
        auto& hand = player->getHand();
        while (hand.size() > 0) {
            hand.removeSpell(0);
        }
        hand.addSpell(std::make_shared<DirectDamageSpell>());
        hand.addSpell(std::make_shared<AreaDamageSpell>());
    } else {
        player = std::make_shared<Player>();
        player->getHand().addSpell(std::make_shared<DirectDamageSpell>());
        player->getHand().addSpell(std::make_shared<AreaDamageSpell>());
    }
    
    field->getCell(0, 0).setPlayer(player);
    
    placeEnemies(field, config.enemyCount, config.enemyHp, config.enemyDamage);
    placeBases(field, config.baseCount);
    placeTowers(field, config.towerCount);
    
    return std::make_unique<GameState>(field, player, level);
}

LevelConfig LevelManager::getLevelConfig(int level) {
    LevelConfig config;

    config.width = 10 + level;
    config.height = 10 + level;
    config.enemyCount = 2 + level * 2;
    config.baseCount = std::max(1, level / 2);
    config.towerCount = std::max(0, level - 1);
    config.wallProbability = 0.1 + level * 0.02;
    config.slowProbability = 0.05 + level * 0.01;
    config.enemyHp = 25 + level * 8;
    config.enemyDamage = 5 + level * 2;
    
    if (config.width > 20) config.width = 20;
    if (config.height > 20) config.height = 20;
    if (config.enemyCount > 15) config.enemyCount = 15;
    if (config.wallProbability > 0.3) config.wallProbability = 0.3;
    if (config.slowProbability > 0.15) config.slowProbability = 0.15;
    if (config.enemyHp > 100) config.enemyHp = 100;
    
    return config;
}

void LevelManager::placeEnemies(std::shared_ptr<Field> field, int count, int hp, int damage) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(1, field->getWidth() - 1);
    std::uniform_int_distribution<> distY(1, field->getHeight() - 1);
    
    for (int i = 0; i < count; ++i) {
        int attempts = 0;
        while (attempts < 50) {
            int x = distX(gen);
            int y = distY(gen);
            
            if (x > 2 || y > 2) {
                if (field->getCell(x, y).isEmpty() && field->getCell(x, y).isPassable()) {
                    auto enemy = std::make_shared<Enemy>(hp, damage);
                    field->getCell(x, y).setEnemy(enemy);
                    break;
                }
            }
            attempts++;
        }
    }
}

void LevelManager::placeBases(std::shared_ptr<Field> field, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(3, field->getWidth() - 1);
    std::uniform_int_distribution<> distY(3, field->getHeight() - 1);
    
    for (int i = 0; i < count; ++i) {
        int attempts = 0;
        while (attempts < 50) {
            int x = distX(gen);
            int y = distY(gen);
            
            if (field->getCell(x, y).isEmpty() && field->getCell(x, y).isPassable()) {
                auto base = std::make_shared<EnemyBase>(3 + i); // Разный интервал спавна
                field->getCell(x, y).setBase(base);
                break;
            }
            attempts++;
        }
    }
}

void LevelManager::placeTowers(std::shared_ptr<Field> field, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(2, field->getWidth() - 1);
    std::uniform_int_distribution<> distY(2, field->getHeight() - 1);
    
    for (int i = 0; i < count; ++i) {
        int attempts = 0;
        while (attempts < 50) {
            int x = distX(gen);
            int y = distY(gen);
            
            if (field->getCell(x, y).isEmpty() && field->getCell(x, y).isPassable()) {
                auto tower = std::make_shared<EnemyTower>(4 + i, 8 + i * 2, 2);
                field->getCell(x, y).setTower(tower);
                break;
            }
            attempts++;
        }
    }
}

std::pair<int, int> LevelManager::findEmptyCell(std::shared_ptr<Field> field) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, field->getWidth() - 1);
    std::uniform_int_distribution<> distY(0, field->getHeight() - 1);
    
    for (int attempt = 0; attempt < 100; ++attempt) {
        int x = distX(gen);
        int y = distY(gen);
        
        if (field->getCell(x, y).isEmpty() && field->getCell(x, y).isPassable()) {
            return {x, y};
        }
    }
    
    return {-1, -1};
}