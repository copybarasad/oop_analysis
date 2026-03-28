#ifndef PLAYER_H
#define PLAYER_H
#include "Position.h"
#include "CombatMode.h"
#include "SpellHand.h"
#include <iostream>

class Player {
private:
    Position position;
    int health = 100;
    int maxHealth = 100;
    int meleeDamage = 25;
    int rangedDamage = 15;
    int score = 0;
    CombatMode combatMode;
    int mana = 100;
    int maxMana = 100;
    SpellHand spellHand;

public:
    Player(int startX, int startY);
    
    Player(const Player& other) = default;            
    Player(Player&& other) = default;                
    Player& operator=(const Player& other) = default; 
    Player& operator=(Player&& other) = default;      
    ~Player() = default;                             
    
    Position getPosition() const { return position; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; } 
    int getScore() const { return score; }
    CombatMode getCombatMode() const { return combatMode; }
    int getDamage() const;
    const char* getCombatModeString() const;
    int getAttackRange() const;

    SpellHand& getSpellHand() { return spellHand; }
    const SpellHand& getSpellHand() const { return spellHand; }
    int getMana() const { return mana; }
    int getMaxMana() const { return maxMana; } 
    void spendMana(int amount);
    void restoreMana(int amount);

    bool canCastSpell(const Spell& spell) const;
    void move(int deltaX, int deltaY);
    void takeDamage(int damageAmount);
    void addScore(int points);
    void switchCombatMode();
    bool isAlive() const { return health > 0; }
    void setPosition(const Position& newPosition) { position = newPosition; }

    int getMeleeDamage() const { return meleeDamage; }
    int getRangedDamage() const { return rangedDamage; }
    void setHealth(int newHealth) { health = newHealth; }
    void setMaxHealth(int newMaxHealth) { maxHealth = newMaxHealth; }
    void setScore(int newScore) { score = newScore; }

    void setCombatMode(CombatMode newMode) { combatMode = newMode; }
    void setMana(int newMana) { mana = newMana; }
    void setMaxMana(int newMaxMana) { maxMana = newMaxMana; }
    void setMeleeDamage(int newDamage) { meleeDamage = newDamage; }
    void setRangedDamage(int newDamage) { rangedDamage = newDamage; }
    
    void initializeWithStarterSpell();
};

#endif