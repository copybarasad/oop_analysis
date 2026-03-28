#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "../utils/Point.h"
#include "CombatMode.h"
#include "SpellHand.h"
#include <iostream>

namespace Game {

class Player {
public:
    Player(int initialHealth = 200, int initialMeleeDamage = 30, int initialRangedDamage = 25);

    void move(const Utils::Point& newPosition);
    void takeDamage(int damageAmount);
    void switchCombatMode();
    void addScore(int points);

    int getHealth() const;
    int getCurrentDamage() const;
    const Utils::Point& getPosition() const;
    void addTurnsInPlace(int turns);
    void resetTurnsInPlace();
    int getTurnsInPlace() const;
    int getScore() const;
    bool isAlive() const;
    CombatMode getCombatMode() const;

    bool canMoveThisTurn() const;
    void setCanMoveThisTurn(bool state);

    void setSlowedForNextTurn(bool state);
    bool isSlowedForNextTurn() const;
    void resetTurnState();
    
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    void addEnhancementStack();
    int getEnhancementStacks() const;
    void resetEnhancementStacks();
    
    void onEnemyKilled();
    bool shouldReceiveNewSpell() const;
    void resetKillTracker();

    void restoreHealth();
    void upgrade(int choice);
    void resetScore();

    void serialize(std::ostream& os) const;
    void deserialize(std::istream& is);

private:
    Utils::Point position;
    int health;
    int maxHealth;
    int score;
    int meleeDamage;
    int rangedDamage;
    CombatMode currentCombatMode;
    bool canMoveFlag;
    bool slowedForNextTurn;
    int turnsInPlace;

    SpellHand spellHand;
    int enhancementStacks;
    int enemiesKilledTracker;
};

} 

#endif