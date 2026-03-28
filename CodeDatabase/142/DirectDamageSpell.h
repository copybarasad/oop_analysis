#pragma once
#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int radius;
    int damage;

public:
    DirectDamageSpell(int radius = 3, int damage = 20);
    
    DirectDamageSpell(const DirectDamageSpell& other) = default;
    DirectDamageSpell& operator=(const DirectDamageSpell& other) = default;
    DirectDamageSpell(DirectDamageSpell&& other) noexcept = default;
    DirectDamageSpell& operator=(DirectDamageSpell&& other) noexcept = default;
    
    bool cast(Player* caster, int targetX, int targetY, Field& field) override;
    const char* getName() const override;
    const char* getDescription() const override;
    std::unique_ptr<Spell> clone() const override;
    int getRadius() const override;
    int getDamage() const override;
    bool isInRange(Player* caster, int targetX, int targetY) const override;
};

