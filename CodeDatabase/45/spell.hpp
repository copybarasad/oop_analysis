#ifndef SPELL_HPP
#define SPELL_HPP

#include "map.hpp"

enum class SpellType {
    Direct,
    Area,
    Trap
};

class Spell {
private:
    SpellType type;
    //int improvment;
    
public:
    Spell(SpellType sp_type);
    virtual ~Spell() = default;
    SpellType getSpellType();
    //void improve();
    virtual bool cast(Map* map) = 0;
};


#endif