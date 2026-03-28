#ifndef OOP_TRAP_SPELL_H
#define OOP_TRAP_SPELL_H

#include "i_spell.h"
#include <string>


class TrapSpell : public ISpell {
    std::string name;
    int damage;

public:
    explicit TrapSpell(std::string  name = "Bear Trap", int damage = 25);

    void cast(Player& caster, GameState& game_state) override;
    std::string get_name() const override;
};

#endif // OOP_TRAP_SPELL_H