#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Spell.h"
#include "SpellHand.h"

class Player : public Character{
private:
    int score;
    SpellHand hand;

public:
    Player(int playerId, const std::string& playerName, int initialHP, 
        CombatStyle style, int startX, int startY);

    int getScore() const;
    void addScore(int points);
    const SpellHand& getHand() const;
    SpellHand& getHand();
    
    void switchStyle();

    void move(int deltaX, int deltaY) override;
    void attack(Character& target) override;
    bool canMoveTo(int targetX, int targetY) const override;

    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
};

#endif