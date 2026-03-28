#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"
#include <nlohmann/json.hpp>

class TrapSpell : public Spell{
private:
    int range;
public:
    TrapSpell(int spell_type, std::string spell_name, int cost, int d, int r);
    int getDamage() const;
    int getRange();

    nlohmann::json toJson() const override;
    void fromJson(const nlohmann::json& j) override;
    std::unique_ptr<Spell> clone() const;

    void setDamage(int d);

};

#endif