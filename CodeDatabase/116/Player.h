#pragma once

#include "Character.h"
#include "PlayerHand.h"
#include <utility>

class Player : public Character {
public:
    enum class CombatMode {
            MELEE,
            RANGED
        };
private:
    bool slowed;
    int maxHealth;
    int score;
    int meleeDamage;
    int rangedDamage;
    int coins;
    CombatMode combatMode;
    PlayerHand hand;

    std::string combatModeToString(const CombatMode& mode) const;
    CombatMode stringToCombatMode(const std::string& str);
    
public:
    Player(int x = 0, int y = 0,int maxHealth = 100, int meleeDamage = 15, int rangedDamage = 10);
    ~Player() override = default;
    int getScore() const;
    int getDamage() const;
    int getCoins() const;
    int getMaxHealth() const {return maxHealth; }
    CombatMode getCombatMode() const;
    const std::pair<int, int> getPlayerDamages() const;
    PlayerHand& getPlayerHand(){
        return hand;
    }
    const PlayerHand& getPlayerHand() const {
        return hand;
    }

    void setMaxHealth(int health) { maxHealth = health; }
    void setScore(int newScore) { score = newScore; }
    void setCoins(int newCoins) { coins = newCoins; }
    void setCombatMode(CombatMode mode) { combatMode = mode; }
    void setSlowed(bool slow) { slowed = slow; }
    void setMeleeDamage(int damage) { meleeDamage = damage; }
    void setRangedDamage(int damage) { rangedDamage = damage; }

    void addScore(int score_points);
    void switchCombatMode();
    void getSlowedDown();
    void removeSlow();
    void updateCoins(int points);
    bool isSlowed() const;
};