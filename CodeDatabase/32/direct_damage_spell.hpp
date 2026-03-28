#ifndef DIRECT_DAMAGE_SPELL_HPP
#define DIRECT_DAMAGE_SPELL_HPP

#include <string>
#include "spell.hpp"

class DirectDamageSpell : public Spell {
private:
    int damage_;
    int radius_;
public:
    DirectDamageSpell(int damage = 25, int radius = 3);
    std::string GetName() const noexcept override;
    bool UseSpell(Game& game) override;
    bool UseSpell(Game& game, int originX, int originY);
    void Upgrade();
};

#endif 