#pragma once
#include "Entity.h"
#include <cstddef>
#include "../../items/include/DirectDamageSpell.h"

class EnemyTower : public Entity {
public:
    EnemyTower(int hp, int damage, int range, int cooldownPeriod);

    EntityType type() const override;
    int damage() const;
    int range() const;          
    bool ready() const;          // может ли атаковать в этот ход
    void tick();                 // уменьшает кулдаун
    void resetCooldown();

    DirectDamageSpell& spell() { return spell_; }
    const DirectDamageSpell& spell() const { return spell_; }

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
    int cooldownPeriod_{};
    int cooldown_{0};
    DirectDamageSpell spell_;
};
