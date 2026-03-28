#ifndef PLAYER_H
#define PLAYER_H

#include "game_entity.h"
#include "player_hand.h"
#include <iostream>

enum class CombatMode {
    MELEE,
    RANGED
};

class Player {
private:
    Position position;
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    CombatMode combatMode;
    int meleeRange = 1;
    int rangedRange = 3;
    PlayerHand hand_;
    int level_ = 1;

public:
    Player(int startX, int startY, int health = 100);

    Position getPosition() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getScore() const;
    CombatMode getCombatMode() const;
    int getAttackRange() const;
    int getMeleeDamage() const { return meleeDamage; }
    int getRangedDamage() const { return rangedDamage; }
    int getLevel() const { return level_; }

    void move(int deltaX, int deltaY);
    void takeDamage(int damage);
    void addScore(int points);
    void switchCombatMode();
    bool isAlive() const;
    void restoreFullHealth() { health = maxHealth; }

    void upgradeMaxHealth(int delta);
    void upgradeMeleeDamage(int delta);
    void upgradeRangedDamage(int delta);
    void setLevel(int level) { level_ = level; }

    PlayerHand& getHand();
    const PlayerHand& getHand() const;
    void buySpell(int cost = 30);

    void save(std::ostream& os) const;
    void load(std::istream& is);

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = default;
    Player& operator=(Player&&) = default;
};

#endif