#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "spell.h"
#include "area_damage_context.h"
#include <string>

class AreaDamageSpell : public Spell {
private:
    int damage;
    int radius;
    int areaSize;  
    
public:
    AreaDamageSpell(int damage = 8, int radius = 4, int areaSize = 2);
    
    bool apply(const AreaDamageContext& context);
    bool canUse(const AreaDamageContext& context) const;
    
    const char* getName() const override;
    std::unique_ptr<Spell> clone() const override;
    void applyEnhancement() override;
    std::string getSerializedData() const override;
    
    
    int getAreaSize() const;
    void setAreaSize(int newSize);
    int getDamage() const;
    int getRadius() const;
};

#endif