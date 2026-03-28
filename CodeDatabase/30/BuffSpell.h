#pragma once
#include "Spell.h"
#include "../../gamecore/include/UpgradeState.h"

// Просто копит стаки в руке, следующий спелл их использует.
class BuffSpell : public Spell {
public:
    BuffSpell(int charges = 1) : Spell("Buff", SpellType::Buff, charges) {}
    bool canCast(const Manager& manager, EntityId casterId, const Position& target,
                 const UpgradeState* up = nullptr) const override;
    bool cast(Manager& manager, EntityId casterId, const Position& target,
              const UpgradeState* up = nullptr) override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;
};
