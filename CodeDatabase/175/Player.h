#pragma once
#include <string>
#include "Hand.h"

enum class AttackMode {
    MELEE,
    RANGED
};

class Player {
private:
    int hp_;
    int maxHp_;
    int damage_;
    int points_;
    bool isSlowed_;
    AttackMode attackMode_;
    Hand hand_;
    int rangedRange_;
    float slowResistance_;

public:
    Player(int hp = 100, int damage = 10, int points = 0);
    int getHp() const;
    int getMaxHp() const;
    int getDamage() const;
    int getPoints() const;
    bool isAlive() const;
    bool isSlowed() const;
    AttackMode getAttackMode() const;
    const Hand& getHand() const;
    Hand& getHand();
    int getRangedRange() const { return rangedRange_; }
    float getSlowResistance() const { return slowResistance_; }

    void setSlowed(bool slowed);
    void setAttackMode(AttackMode mode);
    void addPoints(int pts);
    void takeDamage(int dmg);
    void heal(int amount);
    void increaseDamage(int amount);
    void increaseMaxHp(int amount);
    void increaseRangedRange(int amount);
    void increaseSlowResistance(float amount);
};