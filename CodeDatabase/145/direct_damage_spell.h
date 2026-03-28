#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.h"
#include "direct_damage_context.h"
#include <string>

class DirectDamageSpell : public Spell {
private:
    int damage;
    int radius;
    
public:
    DirectDamageSpell(int damage = 10, int radius = 3);
    bool apply(const DirectDamageContext& context);
    bool canUse(const DirectDamageContext& context) const;
    
    const char* getName() const override;
    std::unique_ptr<Spell> clone() const override;
    void applyEnhancement() override;
    std::string getSerializedData() const override;
    
    int getDamage() const;
    int getRadius() const;
    void setRadius(int newRadius);
};

#endif