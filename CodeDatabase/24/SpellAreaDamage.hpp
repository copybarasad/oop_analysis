#pragma once
#include "ISpell.hpp"

class SpellAreaDamage : public ISpell {
private:
    int damage;
    int range; // радиус применения (от игрока до центра области)

public:
    SpellAreaDamage(int dmg = 10, int rng = 4);
    bool cast(Field& field, const Pos& casterPos, const Pos& target) override;
    std::string name() const override;

    std::unique_ptr<ISpell> clone() const override;
};
