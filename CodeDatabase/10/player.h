#pragma once
#include "character.h"
#include "hand.h"

class Player : public Character {
public:
    enum class CombatStyle { MELEE, RANGED };

private:
    int score;
    CombatStyle combatStyle;
    int attackStrength;
    int attackRange;
    bool slowed;
    int slowTimer;
    Hand spellHand;

public:
    Player(std::string name, Field& field, int startX, int startY, int handSize);
    ~Player() = default;

    void addScore(int value);
    int getScore() const;
    int getPower() const;
    int getRadius() const;
    CombatStyle getCombatType() const;
    bool isSlowedDown() const;

    void switchCombatStyle(CombatStyle style);
    bool canAttack(int targetX, int targetY, std::string enemyName);
    bool move(int deltaX, int deltaY);
    void applySlowEffect();
    void updateSlowEffect();
    bool moveUp();
    bool moveDown();
    bool moveLeft();
    bool moveRight();
    int castSpell(int spellIndex, int targetX, int targetY, int enemyX, int enemyY);
    bool buyNewSpell();
    void showSpells() const;
    bool removeSpell(int index);
    Hand& getSpellHand();
    
    void setPosition(int newX, int newY);
    void setLives(int newLives);
    void setScore(int newScore);
    void setCombatType(CombatStyle style);
    void upgradeDamage(int increment = constants::PLAYER_DAMAGE_INCREMENT); 
};
