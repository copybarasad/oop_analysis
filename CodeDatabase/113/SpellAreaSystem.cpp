#include "SpellAreaSystem.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <cmath>

std::pair<int, int> SpellAreaSystem::findOptimalArea(const Player& caster, const GameField& field, int range, int areaSize) {
    int playerX = caster.getPositionX();
    int playerY = caster.getPositionY();

    std::pair<int, int> bestArea = { playerX, playerY };
    int maxScore = -1;

    for (int x = 0; x <= field.getWidth() - areaSize; x++) {
        for (int y = 0; y <= field.getHeight() - areaSize; y++) {
            int centerX = x + areaSize / 2;
            int centerY = y + areaSize / 2;
            int distance = std::abs(centerX - playerX) + std::abs(centerY - playerY);

            if (distance <= range) {
                int score = countTargetsInArea(caster, x, y, areaSize);
                if (score > maxScore) {
                    maxScore = score;
                    bestArea = { x, y };
                }
            }
        }
    }

    return bestArea;
}

int SpellAreaSystem::countTargetsInArea(const Player& caster, int startX, int startY, int areaSize) {
    int targetCount = 0;
    int endX = startX + areaSize - 1;
    int endY = startY + areaSize - 1;

    for (const auto& enemy : caster.getEnemies()) {
        if (enemy.isAlive() && isPositionInArea(enemy.getPositionX(), enemy.getPositionY(), startX, startY, areaSize)) {
            targetCount++;
        }
    }

    for (const auto& tower : caster.getTowers()) {
        if (tower.isAlive() && isPositionInArea(tower.getPositionX(), tower.getPositionY(), startX, startY, areaSize)) {
            targetCount++;
        }
    }

    return targetCount;
}

bool SpellAreaSystem::isPositionInArea(int posX, int posY, int areaX, int areaY, int areaSize) {
    return posX >= areaX && posX < areaX + areaSize &&
        posY >= areaY && posY < areaY + areaSize;
}