#ifndef SPELLCARD_H
#define SPELLCARD_H

#include <string>
#include <memory>
#include "ISpell.h"  

class SpellCard : public ISpell {
protected:
    std::string name;
    std::string description;
    int manaCost;
    int value;
    int radius;
    int cooldown;
    int currentCooldown;
    bool checkBaseConditions(const SpellContext& context) const;
    bool isInRange(const SpellContext& context) const;

public:
    SpellCard(const std::string& spellName, const std::string& spellDesc, 
              int cost, int effectValue, int effectRadius, int cd);
    virtual ~SpellCard() = default;
     
    std::string getName() const override { return name; }
    std::string getDescription() const override { return description; }
    int getManaCost() const override { return manaCost; }
    int getValue() const override { return value; }
    int getRadius() const override { return radius; }
    int getCooldown() const override { return cooldown; }
    int getCurrentCooldown() const override { return currentCooldown; }
    
    void updateCooldown() override {
        if (currentCooldown > 0) currentCooldown--;
    }
    
    bool isReady() const override { return currentCooldown == 0; }
    
};
 
class DirectDamageSpell : public SpellCard {
public:
    DirectDamageSpell(const std::string& spellName, const std::string& spellDesc, 
                     int cost, int damage, int range, int cd);
    
    bool isDirectDamage() const override { return true; }
    int getRange() const override { return radius; }
    
    bool cast(SpellContext& context) override;
    
protected:
    virtual void applyDirectDamage(SpellContext& context);
};

class AreaDamageSpell : public SpellCard {
public:
    AreaDamageSpell(const std::string& spellName, const std::string& spellDesc, 
                   int cost, int damage, int areaRadius, int cd);
    
    bool isAreaDamage() const override { return true; }
    int getAreaRadius() const override { return radius; }
    
    bool cast(SpellContext& context) override;
    
protected:
    virtual void applyAreaDamage(SpellContext& context);
};

class TrapSpell : public SpellCard {
public:
    TrapSpell(const std::string& spellName, const std::string& spellDesc, 
             int cost, int trapDamage, int range, int cd);
    
    bool isTrap() const override { return true; }
    int getTrapDamage() const override { return value; }
    
    bool cast(SpellContext& context) override;
    
protected:
    virtual void applyTrap(SpellContext& context);
};
 
class FireballSpell : public DirectDamageSpell {
public:
    FireballSpell();
};

class MagicArrowSpell : public DirectDamageSpell {
public:
    MagicArrowSpell();
};

class IceStormSpell : public AreaDamageSpell {
public:
    IceStormSpell();
};

class MagicTrapSpell : public TrapSpell {
public:
    MagicTrapSpell();
};

#endif