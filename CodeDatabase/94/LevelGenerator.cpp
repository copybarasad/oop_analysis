#include "LevelGenerator.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <iostream>

void LevelGenerator::generateLevel(GameController& game, int levelNumber) {
    game.clearEnemies();
    game.clearTowers();
    
    int baseEnemies = 4 + levelNumber;
    int baseWalls = 20 + levelNumber * 2;
    
    int towers = 1;
    
    generateWalls(game, baseWalls);
    generateEnemies(game, baseEnemies, levelNumber);
    if (towers > 0) {
        generateTowers(game, towers);
    }
}

void LevelGenerator::generateWalls(GameController& game, int wallCount) {
    GameField& field = game.getField();
    
    for (int i = 0; i < wallCount; ++i) {
        Position pos = findEmptyPosition(game);
        if (field.isValidPosition(pos)) {
            field.updateCell(pos, CellType::WALL);
        }
    }
}

void LevelGenerator::generateEnemies(GameController& game, int enemyCount, int levelNumber) {
    // УВЕЛИЧИВАЕМ HP ВРАГОВ НА 5 ЗА УРОВЕНЬ
    int baseHealth = 25 + (levelNumber - 1) * 5;
    int baseDamage = 10; // ФИКСИРОВАННЫЙ УРОН 10
    
    for (int i = 0; i < enemyCount; ++i) {
        Position pos = findEmptyPosition(game);
        if (game.getField().isValidPosition(pos)) {
            // У всех врагов одинаковый урон 10
            game.addEnemy(pos, baseHealth, baseDamage);
        }
    }
}

void LevelGenerator::generateTowers(GameController& game, int towerCount) {
    for (int i = 0; i < towerCount; ++i) {
        Position pos = findEmptyPosition(game);
        if (game.getField().isValidPosition(pos)) {
            int cooldown = rng() % 3;
            game.addTower(pos, cooldown);
        }
    }
}

Position LevelGenerator::findEmptyPosition(GameController& game) {
    const GameField& field = game.getField();
    std::uniform_int_distribution<int> distX(0, field.getWidth() - 1);
    std::uniform_int_distribution<int> distY(0, field.getHeight() - 1);
    
    Position pos;
    int attempts = 0;
    
    do {
        pos = Position(distX(rng), distY(rng));
        attempts++;
    } while ((!field.isPositionEmpty(pos) || 
              field.hasCharacterAt(pos) || 
              field.hasTowerAt(pos)) && attempts < 100);
    
    return pos;
}