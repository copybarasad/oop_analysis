#include "EnemyTower.h"

// ============================================================================
// РЕАЛИЗАЦИЯ КЛАССА ENEMYTOWER (ВРАЖЕСКАЯ БАШНЯ)
// ============================================================================

// Конструктор башни
EnemyTower::EnemyTower(const Position& pos) 
    : position(pos), damage(10), attackRange(3), cooldown(5), currentCooldown(0) {}

// Метод атаки башни
void EnemyTower::attack() {
    // Устанавливаем кулдаун после атаки
    currentCooldown = cooldown;
}

// Обновление кулдауна башни
void EnemyTower::updateCooldown() {
    // Уменьшаем кулдаун каждый ход, но не ниже 0
    if (currentCooldown > 0) {
        currentCooldown--;
    }
}