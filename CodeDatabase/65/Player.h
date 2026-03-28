#pragma once

#include "Entity.h"
#include "GameObject.h"
#include "Hand.h"
#include "Config.h"

enum AttackMode {
    CLOSE_COMBAT,
    RANGED_COMBAT,
};

class Player: public Entity {
private:
    Hand hand;
    int xp;
    int level;
    AttackMode attackMode = CLOSE_COMBAT;
    int rangeDamage;
    int range;
    int xpToNextLevel;
    int xpPerKill;

public:
    Player(int x, int y)
        : Entity(x, y, Config::Player::SPRITE, Config::Player::COLOR, 
                Config::Player::HEALTH, Config::Player::DAMAGE, 
                Config::Player::SPEED, ObjectTag::TAG_PLAYER),
          hand(Config::Player::HAND_SIZE), xp(0), level(1), rangeDamage(Config::Player::RANGE_DAMAGE), 
          range(Config::Player::RANGE), xpToNextLevel(100), xpPerKill(50) {}


    int getRangeDamage() const { return rangeDamage; }
    int getRange() const { return range; }
    AttackMode getAttackMode() const { return attackMode; }
    Hand& getHand() { return hand; }
    int getXP() const { return xp; }
    int getLevel() const { return level; }
    int getXPToNextLevel() const { return xpToNextLevel; }
    int getXPPerKill() const { return xpPerKill; }

    // Setters
    void setRangeDamage(int rd) { rangeDamage = rd; }
    void setRange(int r) { range = r; }
    void setXP(int x) { xp = x; }
    void setLevel(int l) { level = l; }
    void setXPToNextLevel(int x) { xpToNextLevel = x; }
    void setAttackMode(AttackMode m) { attackMode = m; }

    void addXP(int amount) {
        xp += amount;
    }

    bool canLevelUp() {
        return xp >= xpToNextLevel;
    }

    void levelUp() {
        if (!canLevelUp()) return;
        xp -= xpToNextLevel;
        level++;
        xpToNextLevel = 100 * level;
        
        // Bonuses
        maxHealth += 10;
        health = maxHealth;
        damage += 5;
        rangeDamage += 10;
    }

    void restoreHealth() {
        health = maxHealth;
    }

    void removeHalfSpells() {
        int spellsToRemove = hand.getSize() / 2;
        for (int i = 0; i < spellsToRemove; i++) {
            // Remove random spell
            if (hand.getSize() > 0) {
                hand.removeSpell(rand() % hand.getSize());
            }
        }
    }

    void upgradeHealth(int amount) {
        maxHealth += amount;
        health = maxHealth;
    }

    void upgradeDamage(int amount) {
        damage += amount;
    }

    void upgradeRangeDamage(int amount) {
        rangeDamage += amount;
    }

    void upgradeRange(int amount) {
        range += amount;
    }

    bool isInAttackRange(int cellX, int cellY) {
        int dx = abs(cellX - x);
        int dy = abs(cellY - y);
        return dx*dx + dy*dy <= range * range;
    }

    bool castSpell(int spellIndex, Field& field, int targetX, int targetY);
    void attack(Entity* target);
    void toggleAttackMode();
    std::unique_ptr<GameObject> clone() const override;
};
