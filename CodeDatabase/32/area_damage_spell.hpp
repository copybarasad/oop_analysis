#ifndef AREA_DAMAGE_SPELL_HPP
#define AREA_DAMAGE_SPELL_HPP

#include "spell.hpp"
#include <string>

class AreaDamageSpell : public Spell{
private:
    int damage_;
    int radius_;

public:
    AreaDamageSpell(int damage = 15, int radius = 3);
    std::string GetName() const noexcept override;
    bool UseSpell(Game& game) override;
    void Upgrade(); 
};

#endif