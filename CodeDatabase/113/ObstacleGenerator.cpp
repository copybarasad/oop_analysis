#include "ObstacleGenerator.h"
#include <iostream>

ObstacleGenerator::ObstacleGenerator() : m_randomGen(std::random_device{}()) {}

void ObstacleGenerator::generateObstacles(GameField& field,
    const std::vector<Enemy>& enemies,
    const std::vector<EnemyTower>& towers,
    int level) {

    field.clear();

    std::uniform_int_distribution<> distX(0, field.getWidth() - 1);
    std::uniform_int_distribution<> distY(0, field.getHeight() - 1);

    int totalCells = field.getWidth() * field.getHeight();
    int obstacleCount = static_cast<int>(totalCells * 0.2);

    int placed = 0;
    int attempts = 0;
    int maxAttempts = totalCells * 5; // ”величиваем количество попыток

    while (placed < obstacleCount && attempts < maxAttempts) {
        int x = distX(m_randomGen);
        int y = distY(m_randomGen);
        attempts++;

        if (isValidObstaclePosition(x, y, field, enemies, towers)) {
            // ѕровер€ем соседние клетки
            bool hasAdjacentWall = false;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;

                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < field.getWidth() &&
                        ny >= 0 && ny < field.getHeight()) {
                        if (field.getCell(nx, ny) == '#') {
                            hasAdjacentWall = true;
                            break;
                        }
                    }
                }
                if (hasAdjacentWall) break;
            }

            // –азрешаем ставить стену только если р€дом нет других стен
            if (!hasAdjacentWall) {
                field.setCell(x, y, '#');
                placed++;
            }
        }
    }

    std::cout << "Generated " << placed << " random obstacles for level " << level << std::endl;
}

bool ObstacleGenerator::isValidObstaclePosition(int x, int y, const GameField& field,
    const std::vector<Enemy>& enemies,
    const std::vector<EnemyTower>& towers) const {

    // ќсновные проверки
    if (!field.isWithinBounds(x, y) || field.getCell(x, y) != ' ') {
        return false;
    }

    if (isPositionOccupied(x, y, enemies, towers)) {
        return false;
    }

    if (isNearPlayerStart(x, y)) {
        return false;
    }

    // ƒополнительно: провер€ем, что р€дом нет других стен (чтобы избежать кластеров)
    int adjacentWalls = 0;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (field.isWithinBounds(nx, ny) && field.getCell(nx, ny) == '#') {
                adjacentWalls++;
            }
        }
    }

    // ќграничиваем количество соседних стен
    return adjacentWalls <= 1;
}

bool ObstacleGenerator::isPositionOccupied(int x, int y,
    const std::vector<Enemy>& enemies,
    const std::vector<EnemyTower>& towers) const {
    for (const auto& enemy : enemies) {
        if (enemy.getPositionX() == x && enemy.getPositionY() == y) {
            return true;
        }
    }

    for (const auto& tower : towers) {
        if (tower.getPositionX() == x && tower.getPositionY() == y) {
            return true;
        }
    }

    return false;
}

bool ObstacleGenerator::isNearPlayerStart(int x, int y) const {
    return (x >= 0 && x <= 2 && y >= 0 && y <= 2);
}