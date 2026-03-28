#ifndef PLAYER_H
#define PLAYER_H

#include "combat_mode.h"
#include "spell_hand.h"

class Player {
private:
    static const int BASE_MAX_HEALTH = 15;
    static const int RANGED_ATTACK_RANGE = 3;
    static const int HEAL_AMOUNT = 1;

    int maxHealth;
    int health;
    int meleeDamage;
    int rangedDamage;
    int score;
    int positionX;
    int positionY;
    CombatMode combatMode;
    SpellHand spellHand;
    
public:
    Player(int startX, int startY);

    void restoreFullHealth();
    void setHealth(int newHealth);
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    int getScore() const;
    int getPositionX() const;
    int getPositionY() const;

    CombatMode getCombatMode() const;

    void takeDamage(int damageAmount);
    void heal();
    void addScore(int points);
    void move(int deltaX, int deltaY);
    void switchCombatMode();

    bool canAttack(int targetX, int targetY) const;
    bool isAlive() const;

    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;

    void increaseMaxHealth(int amount);
    void increaseMeleeDamage(int amount);
    void increaseRangedDamage(int amount);
    void increaseManaCapacity(int amount);
    void enhanceSpellPower();
};

#endif