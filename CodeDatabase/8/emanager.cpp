#include <iostream>
#include <algorithm>
#include "emanager.h"
using namespace std;
void EnemyManager::addEnemy(const Enemy& enemy) {
    enemies.push_back(enemy);
}

Enemy* EnemyManager::getEnemyAt(int x, int y) {
    for (auto& enemy : enemies) {
        if (enemy.getX() == x && enemy.getY() == y && enemy.isAlive()) {
            return &enemy;
        }
    }
    return nullptr;
}
    
vector<Enemy*> EnemyManager::getEnemiesInRadius(int x, int y, int radius) {
    vector<Enemy*> result;
    for (auto& enemy : enemies) {
        int dist = max(abs(enemy.getX() - x), abs(enemy.getY() - y));
        if (dist <= radius && enemy.isAlive()) {
            result.push_back(&enemy);
        }
    }
    return result;
}
    
void EnemyManager::removeDeadEnemies() {
    vector<Enemy> aliveEnemies;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            aliveEnemies.push_back(enemy);  // Используем конструктор копирования, тк ссылки не могут быть переприсвоены      }
        }
    }
    enemies = std::move(aliveEnemies); //перемещает весь вектор, не требуя поэлементного присваивания
}
