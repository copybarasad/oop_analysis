#ifndef AREA_SPELL_H
#define AREA_SPELL_H

#include "Spell.h"
#include <nlohmann/json.hpp>

class AreaSpell : public Spell{
private:
    int area_x;
    int area_y;
    int range;
public:
    AreaSpell(int spell_type, std::string spell_name, int cost, int x, int y, int d, int r);
    int getAreaX() const;
    int getAreaY() const;
    int getDamage() const;
    int getRange() const;
    void setDamage(int d);
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
    std::unique_ptr<Spell> clone() const;

};

#endif