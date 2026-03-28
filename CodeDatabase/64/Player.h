#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "SpellHand.h"

class Enemy;
class GameField;

enum CombatStyle {
    MELEE_STYLE,
    RANGED_STYLE
};

class Player : public Character {
private:
    int totalScore;
    CombatStyle currentCombatStyle;
    SpellHand spellHand;

public:
    Player(int startX, int startY);
    
    void addToScore(int points = 1);
    int getTotalScore() const;
    
    void changePosition(int newX, int newY) override;
    
    void toggleCombatStyle();
    CombatStyle getCurrentCombatStyle() const;
    
    bool isWithinRangedDistance(int targetX, int targetY) const;
    
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
};

#endif