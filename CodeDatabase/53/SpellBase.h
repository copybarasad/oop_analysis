#ifndef SPELLBASE_H
#define SPELLBASE_H

#include "ISpell.h"
#include <string>

class SpellBase : public virtual ISpell { 
protected:
    std::string name;
    int level;
    int damage;
    
public:
    SpellBase(const std::string& spell_name, int spell_level, int base_damage);
    
    std::string getName() const override;
    int getLevel() const override;
    
    int getDamage() const;
    void setLevel(int new_level);
    void setDamage(int new_damage);
};

#endif