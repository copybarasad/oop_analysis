#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "damage_spell.h"

class AreaDamageSpell : public DamageSpell {
public:
    AreaDamageSpell(const std::string& name,
        const std::string& description,
        int damage,
        int range,
        int areaSize,
        GameController* gameController);

    std::vector<Position> getAreaCells(const Position& center) const;

    uint8_t getType() const;
    void save(std::ostream& out) const override;
    void load(std::istream& in) override;

private:
    bool applyEffect() override;
    int areaSize;
};

#endif

