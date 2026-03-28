#pragma once
#include "Entity.h"
#include "Hand.h"

// Forward declaration
class Field;

class Player : public Entity {
private:
    int meleeDamage;
    int rangedDamage;
    CombatMode combatMode;
    int score;
    Hand hand;

public:
    Player(int startX, int startY, int h = 100, int melee = 15, int ranged = 5, int handSize = 5);
    
    Player(const Player& other);
    Player& operator=(const Player& other);
    Player(Player&& other) noexcept;
    Player& operator=(Player&& other) noexcept;

    bool canMove(int dx, int dy, const Field& field) const;
    bool move(int dx, int dy, Field& field);
    bool attack(int targetX, int targetY, Field& field);
    
    void switchCombatMode();
    CombatMode getCombatMode() const;
    int getDamage() const override;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    int getScore() const;
    void addScore(int points);
    void setScore(int score);
    
    // Spell-related methods
    Hand& getHand();
    const Hand& getHand() const;
    bool castSpell(int spellIndex, int targetX, int targetY, Field& field);
    
    char getDisplayChar() const override;
};
