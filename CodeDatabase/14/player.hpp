#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"
#include "defines.hpp"
#include "hand.hpp"
#include <iostream>

class Player : public Entity {
public:
    enum AttackType { MELEE, RANGED };

private:
    int health;
    int score;
    AttackType current_attack_type;
    bool is_slowed;
    Hand* player_hand;
    int kill_count;
    int enhancement_stacks;
    
    int damage_bonus;

public:
    Player(int x, int y, int start_health = PLAYER_START_HEALTH);
    ~Player();

    int getHealth() const override;
    int getScore() const;
    int getDamage() const;
    double getAttackRange() const;
    bool isAlive() const;
    AttackType getAttackType() const;
    bool isSlowed() const;

    void takeDamage(int damage) override;
    void addScore(int points);
    void removeScore(int points);
    void switchAttackType();
    void setSlowed(bool slowed);
    
    Hand* getHand() const;
    void incrementKillCount();
    int getKillCount() const;
    void resetKillCount();
    
    void addEnhancementStack();
    int getEnhancementStacks() const;
    void clearEnhancementStacks();
    
    void upgradeDamage(int bonus);

    EntityType getEntityType() const override { return EntityType::PLAYER; }

    virtual void removeFromBoard(GameBoard& board) override;

    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
};

#endif