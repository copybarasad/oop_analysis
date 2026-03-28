#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "BattleType.h"
#include "Hand.h"

class Player : public Character {
private:
    int meleeDamage;
    int rangedDamage;
    int score;
    BattleType currentBattleType;
    bool skipNextTurn;
    Hand hand;
    int spellsCast;

public:
    Player(int startX, int startY, int initialHealth, int maxX, int maxY, int handSize = 5);

    int getMeleeDamage() const;

    int getRangedDamage() const;

    int getScore() const;

    BattleType getBattleType() const;

    bool shouldSkipTurn() const;

    void addScore(int points);

    void switchBattleType();

    void setSkipNextTurn(bool skip);

    void setHealth(int health);

    void setMeleeDamage(int damage);

    void setRangedDamage(int damage);

    void setScore(int score);

    void setBattleType(BattleType type);

    Hand &getHand();

    const Hand &getHand() const;

    bool canCastSpells() const;

    void incrementSpellsCast();

    int getSpellsCast() const;

    int getDamage() const override;
};

#endif
