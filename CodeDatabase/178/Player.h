#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "CombatType.h"
#include "Position.h"
#include "AttackResult.h"

class Player : public Character
{
private:
    CombatType currentCombatType;
    int score;
    int meleeDamage;
    int rangedDamage;
    bool canMove;
    int meleeRange;
    int rangedRange;
    int coins;
    int maxHealth; 
public:
    Player(const Position &startPosition, int health, int meleeDmg, int rangedDmg);
    CombatType getCombatType() const;
    void switchCombatType();
    int getScore() const;
    void addScore(int points);
    bool getCanMove() const;
    void setCanMove(bool movable);
    void move(const Position &newPosition) override;
    int getAttackRange() const;
    bool isInAttackRange(const Position &targetPosition, const Position &enemyPosition) const;
    AttackResult attack(Character &target);
    int getMeleeRange() const;
    int getRangedRange() const;
    void addCoins(int c);
    bool trySpendCoins(int c);
    int getCoins() const;
    int getMaxHealth() const { return maxHealth; }
    int getMeleeDamage() const { return meleeDamage; }
    int getRangedDamage() const { return rangedDamage; }
    void setMeleeDamage(int damage) { meleeDamage = damage; if (currentCombatType == CombatType::MELEE) this->damage = damage; }
    void setRangedDamage(int damage) { rangedDamage = damage; if (currentCombatType == CombatType::RANGED) this->damage = damage; }
    void setMaxHealth(int health) { maxHealth = health; }
    void restoreHealth() { health = maxHealth; }
    void upgradeHealth(int amount) { maxHealth += amount; health += amount; }
    void upgradeMeleeDamage(int amount) { meleeDamage += amount; if (currentCombatType == CombatType::MELEE) damage += amount; }
    void upgradeRangedDamage(int amount) { rangedDamage += amount; if (currentCombatType == CombatType::RANGED) damage += amount; }
    void resetScore() { score = 0; }
};

#endif