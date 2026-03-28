#pragma once
#include "ISpell.hpp"

class SpellDirectDamage : public ISpell {
private:
    int damage;
    int range; // допустимый радиус применения

public:
    SpellDirectDamage(int dmg = 20, int rng = 3);
    bool cast(Field& field, const Pos& casterPos, const Pos& target) override;
    std::string name() const override;

    std::unique_ptr<ISpell> clone() const override;
};
