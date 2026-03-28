#ifndef CONCRETE_SPELLS_H
#define CONCRETE_SPELLS_H

#include "spell.h"
#include <iostream>
#include <vector>
#include <memory>

class Enemy;
class Player;

class BaseSpell : public Spell {
protected:
    std::string name;
    int damage;
    int range;
    int manaCost;
    int level;

public:
    BaseSpell(const std::string& name, int damage, int range, int manaCost);

    
    std::string getName() const override { return name; }
    int getDamage() const override { return damage; }
    int getRange() const override { return range; }
    int getManaCost() const override { return manaCost; }
    int getLevel() const override { return level; }
    void display() const override;
    
    void cast(int casterX, int casterY, int targetX, int targetY,
              std::vector<Enemy>& enemies, Player& player) override;
    virtual std::unique_ptr<Spell> clone() const override = 0;
    
    void upgrade() override;
    
protected:
    void setDamage(int newDamage) { damage = newDamage; }
    void setRange(int newRange) { range = newRange; }
    bool isInRange(int casterX, int casterY, int targetX, int targetY) const;
    void applyDamageToEnemyAt(int x, int y, std::vector<Enemy>& enemies);
};

class FireballSpell : public BaseSpell {
public:
    FireballSpell(); 
    std::unique_ptr<Spell> clone() const override;
    void cast(int casterX, int casterY, int targetX, int targetY,
              std::vector<Enemy>& enemies, Player& player) override;
};

class IceBladeSpell : public BaseSpell {
public:
    IceBladeSpell();
    std::unique_ptr<Spell> clone() const override;
};

class EnlighteningSpell : public BaseSpell {
public:
    EnlighteningSpell();
    std::unique_ptr<Spell> clone() const override;
    void cast(int casterX, int casterY, int targetX, int targetY,
              std::vector<Enemy>& enemies, Player& player) override;
    void upgrade() override;
};

class IllusionSpell : public BaseSpell {
public:
    IllusionSpell();
    std::unique_ptr<Spell> clone() const override;
};

class TowerBoltSpell : public BaseSpell {
public:
    TowerBoltSpell(); 
    std::unique_ptr<Spell> clone() const override;
    void cast(int casterX, int casterY, int targetX, int targetY,
              std::vector<Enemy>& enemies, Player& player) override;
};

#endif