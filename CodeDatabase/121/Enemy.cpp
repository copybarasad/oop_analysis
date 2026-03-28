#include "Enemy.h"
#include "Player.h"
#include "GameField.h"



// Реализация логики врага
// Содержит методы поведения врага, использует конфигурацию
// для балансировки характеристик.


// Конструктор по умолчанию - использует значения по умолчанию
Enemy::Enemy()
    : Entity(25, Position(0, 0), "Enemy", 10), 
      damage(5), attackRange(1.5) {}

// Конструктор с позицией - использует значения по умолчанию
Enemy::Enemy(const Position& pos)
    : Entity(25, pos, "Enemy", 10), 
      damage(5), attackRange(1.5) {}

// Конструктор с конфигурацией - использует переданные настройки
Enemy::Enemy(const Position& pos, const EnemyConfig& config)
    : Entity(config.health, pos, "Enemy", config.reward),
      damage(config.damage), attackRange(config.attackRange) {}

// Конструктор с полными параметрами
Enemy::Enemy(int health, int damage, const Position& pos, const EnemyConfig& config)
    : Entity(health, pos, "Enemy", config.reward),
      damage(damage), attackRange(config.attackRange) {}

// Метод получения урона - вызывает базовую реализацию из Entity
void Enemy::takeDamage(int damageAmount) {
    Entity::takeDamage(damageAmount);
}

// Преследование игрока - логика будет в AISystem
void Enemy::chasePlayer(Player& player, const GameField& field) {
    // Логика будет в AISystem
    (void)player;
    (void)field;
}

// Проверка может ли враг атаковать игрока
bool Enemy::canAttackPlayer(const Player& player) const {
    return distanceTo(player) <= attackRange;
}

// Атака игрока - наносит урон если игрок в радиусе
void Enemy::attackPlayer(Player& player) {
    if (canAttackPlayer(player)) {
        player.takeDamage(damage);
    }
}