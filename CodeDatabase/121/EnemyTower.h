#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "Entity.h"
#include "GameConfig.h"
#include "GameField.h"
#include "Player.h"


// Класс магической башни с заклинаниями
// Атакует игрока магическими заклинаниями с расстояния.
// Использует конфигурацию для настройки урона и перезарядки.


class EnemyTower : public Entity {
private:
    int attackRange;           // Радиус атаки башни
    int spellDamage;           // Урон заклинания
    int spellCooldown;         // Время перезарядки заклинания
    int currentSpellCooldown;  // Текущее время до следующего заклинания
    bool canCastSpell;         // Флаг может ли башня кастовать

public:
    // Конструктор с позицией и конфигурацией
    EnemyTower(const Position& pos, const TowerConfig& config);
    
    // Геттеры параметров башни
    int getAttackRange() const { return attackRange; }
    int getSpellDamage() const { return spellDamage; }
    int getSpellCooldown() const { return spellCooldown; }
    int getCurrentSpellCooldown() const { return currentSpellCooldown; }
    bool canCast() const { return canCastSpell && isAlive(); }
    
    // Методы обновления и атаки
    void update() override;
    bool tryCastSpell(Player& player, GameField& field);
    void takeDamage(int damageAmount) override;
    
private:
    // Проверка находится ли игрок в радиусе атаки
    bool isPlayerInRange(const Player& player) const;
};

#endif