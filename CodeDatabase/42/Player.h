#pragma once
#include "PlayerHand.h"

enum class AttackType {
    MELEE,
    RANGED
};

class Player {
private:
    int health;
    int maxHealth;
    AttackType currentAttackType;
    PlayerHand hand;
    int enemiesKilled;
    int enemiesKilledLastSpell;
    
public:
    Player(int initialHealth, int handSize = 5);
    
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    bool isAlive() const;
    AttackType getAttackType() const;
    
    void takeDamage(int damage);
    void heal(int amount);
    void switchAttackType();
    
    PlayerHand& getHand();
    const PlayerHand& getHand() const;
    void incrementEnemyKill();
    int getEnemiesKilled() const;
    int getEnemiesKilledSinceLastSpell() const;
    bool shouldReceiveSpell() const;
    void resetSpellGrantCounter();

    void setEnemiesKilled(int killed) { enemiesKilled = killed; }
    void setEnemiesKilledSinceLastSpell(int killed) { enemiesKilledLastSpell = enemiesKilled - killed; }
};