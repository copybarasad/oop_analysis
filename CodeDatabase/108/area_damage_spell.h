#ifndef OOP_AREA_DAMAGE_SPELL_H
#define OOP_AREA_DAMAGE_SPELL_H

#include "i_spell.h"
#include <string>

class AreaDamageSpell : public ISpell {
    std::string name;
    int damage;

public:
    explicit AreaDamageSpell(std::string name = "Firestorm", int damage = 15);

    void cast(Player& caster, GameState& game_state) override;
    std::string get_name() const override;
};

#endif // OOP_AREA_DAMAGE_SPELL_H