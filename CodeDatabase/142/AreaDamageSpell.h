#pragma once
#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int radius;
    int damage;

public:
    AreaDamageSpell(int radius = 3, int damage = 15);
    
    AreaDamageSpell(const AreaDamageSpell& other) = default;
    AreaDamageSpell& operator=(const AreaDamageSpell& other) = default;
    AreaDamageSpell(AreaDamageSpell&& other) noexcept = default;
    AreaDamageSpell& operator=(AreaDamageSpell&& other) noexcept = default;
    
    bool cast(Player* caster, int targetX, int targetY, Field& field) override;
    const char* getName() const override;
    const char* getDescription() const override;
    std::unique_ptr<Spell> clone() const override;
    int getRadius() const override;
    int getDamage() const override;
    bool isInRange(Player* caster, int targetX, int targetY) const override;
};

