#ifndef GAMELB1_PLAYER_H
#define GAMELB1_PLAYER_H

#include "Creature.h"
#include "PlayerHand.h"
#include <string>
#include <memory>

class Player : public Creature {
public:
    enum class CombatMode {
        kMelee,
        kRanged
    };

    Player(int health, int melee_damage, int ranged_damage, int score, int x, int y);
    Player();

    int getDamage() const override;
    void switchCombatMode();

    int getScore() const;
    void addScore(int points);
    std::string getModeName() const;

    bool isSlowed() const;
    void setSlowed(bool slowed);

    PlayerHand& getHand();
    void addBoostCharge(int amount);
    int consumeBoostCharges();

    int getBoostCharges() const;
    void setBoostCharges(int amount);

    void heal();
    void setMaxHealth(int new_max);
    void increaseDamage(int amount);


private:
    int score_;
    int melee_damage_;
    int ranged_damage_;
    int max_health_;

    CombatMode combat_mode_;
    bool is_slowed_;
    PlayerHand hand_;
    int boost_charges_;
};

#endif //GAMELB1_PLAYER_H