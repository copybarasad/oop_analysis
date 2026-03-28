#ifndef DIRECTED_SPELL_H
#define DIRECTED_SPELL_H

#include "Spell.h"
#include <nlohmann/json.hpp>


class DirectedSpell : public Spell{
private:
    int range;
public:
    DirectedSpell(int spell_type, std::string spell_name, int cost, int d, int r);
    
    int getDamage() const;
    int getRange();
    
    void setDamage(int d);

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
    std::unique_ptr<Spell> clone() const;
};

#endif