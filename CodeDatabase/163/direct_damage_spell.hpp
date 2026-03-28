#ifndef DIRECT_DAMAGE_SPELL_HPP
#define DIRECT_DAMAGE_SPELL_HPP

#include "spell_card.hpp"

class DirectDamageSpell : public SpellCard {
private:
    int damage;

public:
    DirectDamageSpell(const std::string& name, const std::string& desc, 
                     int spellRange, int spellDamage);
    
    void castAsProjectile(GameState* gameState, int dirX, int dirY) const override;
    std::unique_ptr<SpellCard> clone() const override;
};

#endif