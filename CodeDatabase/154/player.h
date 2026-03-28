#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "hand.h"

class Player : public Entity {
public:
    Player(int id, int health, int meleeDamage, int rangedDamage, int score);
    EntityType getType() const override;
    int getDamage() const override;
    int getAttackRange() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    void setMeleeDamage(int damage);
    void setRangedDamage(int damage);
    void toggleCombatMode();
    void storeBuffForNextSpell(int extraRange, int extraArea, int extraTrapDamage, int extraSummons);
    int consumeExtraRange();
    int consumeExtraArea();
    int consumeExtraTrapDamage();
    int consumeExtraSummons();
    void applySlow();
    bool canMove() const;
    void resetSlow();
    void addScore(int amount);
    int getScore() const;
    bool isRanged() const;
    Player* asPlayer() override;
    Hand& hand();
private:
    int extraRange_ = 0;
    int extraArea_ = 0;
    int extraTrapDamage_ = 0;
    int extraSummons_ = 0;

    int meleeDamage_;
    int rangedDamage_;
    int meleeRange_;
    int rangedRange_;
    bool isRanged_;
    bool isSlowed_;
    int score_;
    Hand hand_;
};

#endif
