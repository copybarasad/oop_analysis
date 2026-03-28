#ifndef PLAYER_H
#define PLAYER_H

#include "SpellHand.h"
#include "Position.h"
#include <memory>

class GameEngine;

enum CombatMode {
    MELEE,
    RANGED
};

class Player {
private:
    int health;
    const int maxHealth;
    const int meleeDamage;
    const int rangedDamage;
    CombatMode currentCombatMode;
    int score;
    int mana;
    const int maxMana;
    SpellHand spellHand;

public:
    Player(int initialHealth, int meleeDmg, int rangedDmg, int initialMana, int spellHandSize);
    
    void takeDamage(int damage);
    void switchCombatMode();
    bool isAlive() const;
    int getCurrentDamage() const;
    int getHealth() const;
    int getScore() const;
    void addScore(int points);
    CombatMode getCombatMode() const;
    void setHealth(int health);
    void setScore(int score);
    void setMana(int mana);
    void setCombatMode(CombatMode mode);
    
    bool castSpell(int spellIndex, const Position& targetPosition, GameEngine& game);
    void addMana(int amount);
    int getMana() const;
    int getMaxMana() const;
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    void onEnemyKilled();
    bool canBuySpell() const;
    void buyRandomSpell();
};

#endif