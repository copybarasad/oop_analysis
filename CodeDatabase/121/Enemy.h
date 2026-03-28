#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "GameConfig.h"



// Класс обычного врага
// Содержит логику поведения врага, атаки и преследования игрока.
// Использует конфигурацию для балансировки характеристик.

class Player;
class GameField;

class Enemy : public Entity {
private:
    int damage;           // Урон который наносит враг
    double attackRange;   // Радиус атаки врага

public:
    // Конструктор по умолчанию - использует стандартную конфигурацию
    Enemy();
    
    // Конструктор с позицией - создает врага в указанной позиции
    explicit Enemy(const Position& startPos);
    
    // Конструктор с конфигурацией - использует переданные настройки
    Enemy(const Position& startPos, const EnemyConfig& config);
    
    // Конструктор с полными параметрами
    Enemy(int health, int damage, const Position& pos, const EnemyConfig& config);
    
    // Геттер и сеттер урона
    int getDamage() const { return damage; }
    void setDamage(int newDamage) { damage = newDamage; }
    
    // Геттер радиуса атаки
    double getAttackRange() const { return attackRange; }
    
    // Переопределяем метод получения урона
    void takeDamage(int damageAmount) override;
    
    // Методы поведения врага
    void chasePlayer(Player& player, const GameField& field);
    bool canAttackPlayer(const Player& player) const;
    void attackPlayer(Player& player);
};

#endif