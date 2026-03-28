#pragma once
#include "field.h"
#include "player.h"
#include "tower_spell.h"
#include <string>

class DamageTower {
private:
    std::string name;
    int x, y;
    Field& field;
    int lives;
    TowerSpell spell;
    int cooldownTimer;
    int cooldownDuration;

public:
    DamageTower(std::string name, Field& field, int startX, int startY);
    ~DamageTower() = default;

    bool canAttackPlayer(int playerX, int playerY) const;
    int attackPlayer(int playerX, int playerY);
    void takeDamage(int damage);
    void updateCooldown();
    
    bool isAlive() const;
    std::string getName() const;
    int getX() const;
    int getY() const;
    int getLives() const;
    int getCooldownTimer() const;
    void setLives(int newLives);
    void setCooldown(int newCooldown);
    void upgradeDamage(int increment = constants::TOWER_DAMAGE_INCREMENT);
    int getSpellPower() const { return spell.getPower(); }
};
