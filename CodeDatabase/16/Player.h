#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Hand.h"
#include "Enhancements.h"

enum class CombatStance { kMelee, kRanged };

class Player : public Entity {
public:
    Player(int start_x, int start_y, int health, int melee_damage, int ranged_damage);

    void switchStance();
    CombatStance getStance() const;
    int getRange() const;

    bool isSlowed() const;
    void setSlowed(bool slowed);
    
    Hand& getHand();
    Enhancements& getEnhancements();
    void addScore(int amount);
    int getScore() const;
    bool spendScore(int amount);

    void healFull();
    void increaseMaxHealth(int amount);
    void increaseMeleeDamage(int amount);

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
    CombatStance stance_;
    int melee_damage_;
    int ranged_damage_;
    bool is_slowed_ = false;
    Hand hand_;
    Enhancements enhancements_;
    int score_ = 0;
    int max_health_;
};

#endif