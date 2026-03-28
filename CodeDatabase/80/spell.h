#ifndef SPELL_H
#define SPELL_H

#include <string>
#include "Map.h"

class Spell {
protected:
    std::string info;
    int damage;  
    int range;  

public:
    Spell(int dmg, int rng) : damage(dmg), range(rng) {}
    virtual ~Spell() = default;

    virtual void UseSpell(int target_x, int target_y, Map& map) = 0;
    virtual bool CanCast(int caster_x, int caster_y, int target_x, int target_y, Map& map) = 0;

    virtual std::string GetInfo();
    virtual void SetInfo(std::string info);

    virtual void UpgradeDamage(int increase) {
        damage += increase;
        UpdateInfo(); 
    }

    int GetDamage() const { return damage; }
    int GetRange() const { return range; }

protected:
    virtual void UpdateInfo() = 0;
};

#endif