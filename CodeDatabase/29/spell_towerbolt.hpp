#ifndef SPELL_TOWERBOLT_HPP
#define SPELL_TOWERBOLT_HPP
#include "spell.hpp"
class TowerBoltSpell : public ISpell {
    int damage_;
    int range_;
public:
    TowerBoltSpell(int dmg,int range);
    const char* Name() const noexcept override;
    int Range() const noexcept override;
    bool Use(Game& game,int tx,int ty) override;
    std::string Serialize() const override;
};
#endif
