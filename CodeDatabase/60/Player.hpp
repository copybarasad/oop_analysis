#pragma once
#include <Entity.hpp>
#include <Ceil.hpp>
#include <Constants.hpp>
#include <vector>
#include <LivingEntity.hpp>
#include <Gauntlet.hpp>

class Player : public LivingEntity {
public:
    Player(int x, int y);
    
    bool handleInput(std::vector<std::vector<Ceil>>& ceils, std::vector<std::shared_ptr<LivingEntity>>& entities, int ch, const ScreenSize* screen, int height, int width);
    int attack();
    virtual void draw() override;
    void drawUI() const;
    void resetAttackFlag();
    bool getWasAttacked() const;
    void setWasAttacked(bool val);
    void setTargeted(bool targeted);
    bool isTargeted() const;
    bool getInTrap();
    int getMp() const;
    int getLvl() const;
    int getDamage() const;
    void giveExp(int exp);
    void lvlUp();
    int getExpToLvlUp() const;
    int getMpMax() const;
    virtual int getExp() const override;
    float getScale() const;
    void useSkill(int cost);
    void giveMp(int quantity);
    void setInTrap();
    void unSetInTrap();
    void switchMode();
    bool getMode();
    void addEmpowermentCharge();
    int getEmpowermentCharges() const;
    void clearEmpowermentCharges();
    void setMp(int value);
    void setMpMax(int value);
    void setLvl(int value);
    void setExp(int value) override; 
    void setExpToLvlUp(int value);
    void setDamage(int value);
    void setMode(bool isRange);
    void setEmpowermentCharges(int value);
    void setScale(float value);
    void clearSpells();
    void learnSpell(std::shared_ptr<Spell> newSpell);
    const std::vector<std::shared_ptr<Spell>>& getSpells() const;
    void prepareForNextLevel(int newLevel);
    
private:

    Gauntlet gauntlet;
    int level;
    int expToLvlUp;
    int mp;
    int mpMax;
    int damage;
    float scale;
    bool weaponMode;
    bool isBeingTargeted = false;
    int empowermentCharges = 0;
    bool wasAttacked = false;
    bool inTrap = false;
    char sprite = PLAYER;
};