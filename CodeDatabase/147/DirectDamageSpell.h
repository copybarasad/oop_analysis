#ifndef DIRECTDAMAGE_SPELL_H
#define DIRECTDAMAGE_SPELL_H

#include "basespell.h"

class DirectDamageSpell : public BaseSpell {
public:
    DirectDamageSpell(int damage, int radius)
        : BaseSpell("Direct Damage", damage, radius) {}

    // Метод глубокого копирования (виртуальный конструктор)
    std::unique_ptr<ISpell> clone() const override {
        return std::make_unique<DirectDamageSpell>(*this);
    }

    ~DirectDamageSpell() override = default;
};

#endif // DIRECTDAMAGE_SPELL_H
