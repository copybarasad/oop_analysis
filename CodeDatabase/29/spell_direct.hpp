#ifndef SPELL_DIRECT_HPP
#define SPELL_DIRECT_HPP
#include "spell.hpp"
class DirectDamageSpell : public ISpell {
    int damage_;
    int range_;
public:
    DirectDamageSpell(int dmg,int range);
    const char* Name() const noexcept override;
    int Range() const noexcept override;
    bool Use(Game& game,int tx,int ty) override;
    std::string Serialize() const override;
};
#endif
