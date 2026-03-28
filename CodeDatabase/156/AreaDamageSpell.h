//
// Created by Artem on 24.10.2025.
//

#ifndef LABAOOP2_AREADAMAGESPELL_H
#define LABAOOP2_AREADAMAGESPELL_H
#include "SpellOnCoords.h"
#include <map>
#include <memory>

class AreaDamageSpell : public SpellOnCoords {
public:
    AreaDamageSpell() : SpellOnCoords(SpellType::AreaDamageSpell) {}
    void castSpell(int gradeLevel, Field &field, Constants::XYPair from, Constants::XYPair to) const override;

    std::unique_ptr<Spell> clone() const override;

private:
    struct levelInfo{
        int damage, range, areaRad;
    };
    const std::map<int, levelInfo> levels = {{0, {1,   2, 1}}, //0 - ослабленная вервсия заклинания для тестов
                                             {1, {1,   4, 2}},
                                             {2, {2,   4, 2}},
                                             {3, {3,   4, 2}},
                                             {4, {5, 6, 2}}
    };
    const std::vector<Constants::dxdy> damageCoords = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
};


#endif //LABAOOP2_AREADAMAGESPELL_H
