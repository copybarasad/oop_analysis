#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Constants.h"
#include "PlayerHand.h"

enum class AttackType {
    RANGE,
    MELEE
};

class Player : public Entity {
private:
    int base_dmg;
    AttackType attack_type;
    int score;
    bool is_slowed;
    bool slowed_next_turn;
    PlayerHand* hand;

public:
    Player(int x, int y, int hp = PLAYER_DEFAULT_HP, int base_dmg = PLAYER_DEFAULT_BASE_DMG);
    
    int getBaseDamage() const;
    AttackType getAttackType() const;
    int getScore() const;
    bool getIsSlowed() const;
    int getCurrentDamage() const;
    void switchAttackType();
    void addScore(int points);
    void setSlowed(bool slowed);
    void setSlowedNextTurn(bool slowed);
    bool getSlowedNextTurn() const;
    PlayerHand* getHand() const { return hand; }
    void setBaseDamage(int damage) { base_dmg = damage; }
    void setScore(int newScore);
};

#endif 
