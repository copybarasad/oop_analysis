#include "LevelGenerator.h"
#include <random>
#include <chrono>

void LevelGenerator::generate(int difficulty, 
    Field& field, 
    Player& player, 
    std::vector<Enemy>& enemies,
    std::vector<EnemySpawner>& spawners,
    std::vector<Trap>& traps,
    std::vector<Ally>& allies,
    std::vector<EnemyTower>& towers) 
{
    enemies.clear();
    spawners.clear();
    traps.clear();
    allies.clear();
    towers.clear();

    int newW = 15 + difficulty; 
    int newH = 15 + difficulty;
    if (newW > 25) newW = 25; 
    if (newH > 25) newH = 25;
    
    field = Field(newW, newH);

    int px = newW / 2;
    int py = newH / 2;
    field.setCellType(px, py, CellType::PLAYER);
    player.teleport(px, py);

    int enemyCount = 2 + difficulty;
    int spawnerCount = 1 + difficulty / 3;

    int ex, ey;
    for (int i = 0; i < enemyCount; ++i) {
        if (placeEntityRandomly(field, CellType::ENEMY, ex, ey)) {
            enemies.emplace_back(ex, ey, 50 + difficulty * 10, 5 + difficulty);
        }
    }

    for (int i = 0; i < spawnerCount; ++i) {
        if (placeEntityRandomly(field, CellType::SPAWNER, ex, ey)) {
            spawners.emplace_back(ex, ey, 5, 100 + difficulty * 10);
        }
    }

    if (difficulty >= 2) {
        int towerCount = difficulty / 2;
        for (int i = 0; i < towerCount; ++i) {
            if (placeEntityRandomly(field, CellType::ENEMY_TOWER, ex, ey)) {
                towers.emplace_back(ex, ey, 150);
            }
        }
    }
}

bool LevelGenerator::placeEntityRandomly(Field& field, CellType type, int& outX, int& outY) {
    static std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> distribX(1, field.getWidth() - 2);
    std::uniform_int_distribution<> distribY(1, field.getHeight() - 2);

    bool isPlaced = false;
    int attempts = 0;
    const int max_attempts = 1000;
    while (!isPlaced && attempts < max_attempts) {
        int x = distribX(gen);
        int y = distribY(gen);

        if (field.getCellType(x, y) == CellType::EMPTY) {
            field.setCellType(x, y, type);
            outX = x;
            outY = y;
            isPlaced = true;
        }
        attempts++;
    }
    
    return isPlaced;
}