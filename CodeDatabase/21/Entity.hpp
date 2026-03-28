#pragma once
#include <utility>
#include <memory>
#include <vector>

// #include "../core/GameContext.hpp"
// #include "../core/Config.hpp"
// #include "../core/EntityManager.hpp"
// #include "../core/FieldCell.hpp"
struct GameContext;

class Entity {
public:
    enum class entityType { UNDEFINED, PLAYER, ENEMY, BARRACKS, TOWER };
    virtual ~Entity() = default;
    virtual entityType getType() const = 0;
    virtual void causeDamage(int damage) = 0;
    virtual int getDamage() = 0;
    virtual std::pair<int, int> getHealth() = 0;
    virtual bool alive() const = 0;

    virtual void swapWeapon() {};
    virtual void swapItemInHand(int itemIndex) {(void)itemIndex; };
    virtual bool melle() { return true; };
    virtual bool checkDebaffState() { return false; };
    virtual void setDebaffState() {};
    virtual void regenerateLife(float procrecoveryPercentage) { (void)procrecoveryPercentage;};

    virtual bool timeToSpawn() { return false; };
    virtual void useItem(GameContext& ctx, int userIndex, int power) { (void)ctx; (void)userIndex; (void)power; };
    virtual int getInt() { return 0; };
    virtual int getDex() { return 0; };
    virtual int getStr() { return 0; };
    virtual int getPowerOfSpell() { return 0; };
    virtual void setPowerOfSpell(int newPower) {(void)newPower;};
    virtual void setInt(int newInt) { (void)newInt;};
    virtual void setDex(int newDex) { (void)newDex;};
    virtual void setStr(int newStr) { (void)newStr;};
    virtual int getLevel() { return 1; };
    virtual bool isLevelIncreased() { return false; };
    virtual void addExperience(int exp) { (void)exp; };
    virtual void addSpells(int indexInHand, int count) { (void)indexInHand; (void)count; };
    virtual std::vector<long long> getExperience() { return {0,0,0}; };
    virtual char returnEntitySymbol() = 0;
    virtual void goToTheNextLevel() {};
};