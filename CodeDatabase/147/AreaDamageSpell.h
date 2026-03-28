#ifndef AREADAMAGE_SPELL_H
#define AREADAMAGE_SPELL_H

#include "basespell.h"

class AreaDamageSpell : public BaseSpell {
public:
    AreaDamageSpell(int damage, int radius)
        : BaseSpell("Area Damage", damage, radius) {}

    // Метод глубокого копирования (виртуальный конструктор)
    std::unique_ptr<ISpell> clone() const override {
        return std::make_unique<AreaDamageSpell>(*this);
    }

    ~AreaDamageSpell() override = default;
};

#endif // AREADAMAGE_SPELL_H
