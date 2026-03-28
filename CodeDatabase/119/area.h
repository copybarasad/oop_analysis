#ifndef AREA_H
#define AREA_H

#include "spell.h"
#include "entity.h"

class AreaDamage : public ISpell {
private:
    int dmg;
    int radius;

    std::string name;
    SpellType id;

public:
    AreaDamage(int dmg = 10, int radius = 2, std::string name = "Area Spell", SpellType id = AREA);
    void cast(const CastContext &context) const override;
    std::string getName() const override;
    void upgrade() override;
    SpellType getTypeId() const override { return id; }
    int getDmg() const override { return dmg; }
    void setDmg(int dmg) override { this->dmg = dmg; }
};

#endif