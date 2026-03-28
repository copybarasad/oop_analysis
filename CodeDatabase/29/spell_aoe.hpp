#ifndef SPELL_AOE_HPP
#define SPELL_AOE_HPP
#include "spell.hpp"
class AoESpell2x2 : public ISpell {
    int damage_;
    int range_;
public:
    AoESpell2x2(int dmg,int range);
    const char* Name() const noexcept override;
    int Range() const noexcept override;
    bool Use(Game& game,int tx,int ty) override;
    std::string Serialize() const override;
};
#endif
