#ifndef PLAYER_H
#define PLAYER_H

#include <utility>
#include "SpellHand.h"

enum class CombatMode {
    Melee,
    Ranged
};

class Player {
public:
    Player(int initialHealth, int initialMeleeDamage, int initialRangedDamage, int x, int y, int spellHandSize = 3);

    int getHealth() const;
    void takeDamage(int amount);
    bool isAlive() const;

    int getDamage() const;
    int getMeleeDamage() const { return meleeDamage; }
    int getRangedDamage() const { return rangedDamage; }
    void switchCombatMode();
    void setCurrentCombatMode(CombatMode mode);
    CombatMode getCombatMode() const;
    
    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);

    void addScore(int points);
    void setScore(int points);
    int getScore() const;

    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    void gainSpell();

    void restoreHealth();
    void increaseMeleeDamage(int amount);
    void increaseRangedDamage(int amount);

private:
    int health;
    int meleeDamage;
    int rangedDamage;
    int score;
    CombatMode currentCombatMode;
    std::pair<int, int> position;
    SpellHand spellHand;
};

#endif