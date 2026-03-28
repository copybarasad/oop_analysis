#ifndef SPELL_TRAP_HPP
#define SPELL_TRAP_HPP
#include "spell.hpp"
class TrapSpell : public ISpell {
    int damage_;
    int range_;
public:
    TrapSpell(int dmg,int range);
    const char* Name() const noexcept override;
    int Range() const noexcept override;
    bool Use(Game& game,int tx,int ty) override;
    std::string Serialize() const override;
};
#endif
