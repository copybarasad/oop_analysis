#ifndef AREA_DAMAGE_SPELL_HPP
#define AREA_DAMAGE_SPELL_HPP

#include "spell_card.hpp"

class AreaDamageSpell : public SpellCard {
private:
    int damage;
    int areaSize;

public:
    AreaDamageSpell(const std::string& name, const std::string& desc, 
                   int spellRange, int spellDamage, int size = 2);
    
    void castAsProjectile(GameState* gameState, int dirX, int dirY) const override;
    std::unique_ptr<SpellCard> clone() const override;
};

#endif