#ifndef PLAYER_H
#define PLAYER_H

#include "position.h"
#include "game_enums.h"
#include "spell_hand.h"

class Player {
private:
    int health;
    int meleeDamage;
    int rangedDamage;
    int score;
    CombatMode currentMode;
    Position position;
    bool canMove;
    int mana;
    int maxMana;
    SpellHand spellHand;
    
    void validateHealth();
    void validateMana();

public:
    Player(int startHealth = 100, int startMeleeDamage = 20, int startRangedDamage = 10);
    
    int getHealth() const;
    int getDamage() const;
    int getScore() const;
    CombatMode getCombatMode() const;
    Position getPosition() const;
    bool getCanMove() const;
    int getMana() const;
    int getMaxMana() const;
    bool canCastSpell(int manaCost) const;
    
    const SpellHand& getSpellHand() const;
    SpellHand& getSpellHand() { return spellHand; }
    
    void setPosition(const Position& newPosition);
    void setCanMove(bool movable);
    void setMana(int newMana);
    
    void takeDamage(int damage);
    void addScore(int points);
    void restoreMana(int amount);
    void switchCombatMode();
    void onEnemyDefeated();
    bool isAlive() const;
    
    void heal(int amount) {
        if (amount > 0) {
            health += amount;
            validateHealth();
        }
    }

    void setScore(int newScore) { if (newScore >= 0) score = newScore; }
};

#endif