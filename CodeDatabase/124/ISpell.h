#ifndef ISPELL_H
#define ISPELL_H

#include <string>
#include "SpellContext.h"

class ISpell {
public:
    virtual ~ISpell() = default;
      
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual bool cast(SpellContext& context) = 0;
     
    virtual int getValue() const { return 0; }           
    virtual int getRadius() const { return 0; }          
    virtual int getCooldown() const { return 0; }         
    virtual int getCurrentCooldown() const { return 0; }  
     
    virtual bool isDirectDamage() const { return false; }
    virtual bool isAreaDamage() const { return false; }
    virtual bool isTrap() const { return false; }
     
    virtual int getDamage() const { return getValue(); }
    virtual int getTrapDamage() const { return getValue(); }
    virtual int getAreaRadius() const { return getRadius(); }
    virtual int getRange() const { return getRadius(); }
     
    virtual void updateCooldown() {}
    virtual bool isReady() const { return true; }
};

#endif