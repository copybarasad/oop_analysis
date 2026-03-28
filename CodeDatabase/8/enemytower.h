#pragma once
#include "field.h"
#include "player.h"
#include "spell.h"
#include "directspell.h"
#include "towerspell.h"
#include <string>

class EnemyTower {
private:
    std::string name;
    int x, y;
    int attackRange;
    bool canAttack;
    int cooldown;
    int currentCooldown;
    TowerDirectSpell spell;

public:
    EnemyTower(std::string name, int startX, int startY);
    ~EnemyTower() = default;
    void update(Player& player, std::string& message);
    bool tryAttackPlayer(Player& player, std::string& message);
    std::string getName() const;
    int getX() const;
    int getY() const;
    int getAttackRange() const;
    bool canAttackNow() const;
    void startCooldown(std::string& message);

    //сеттеры для загрузки
    void setPosition(int newX, int newY) { 
        x = newX; 
        y = newY; 
    }
    void setCanAttack(bool attack) { canAttack = attack; }
    void setCurrentCooldown(int cooldown) { currentCooldown = cooldown; }
    void setAttackRange(int range) { attackRange = range; }
    void setName(std::string newName) { name = newName; }
};