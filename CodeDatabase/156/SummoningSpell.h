//
// Created by Artem on 30.10.2025.
//

#ifndef LABAOOP2_SUMMONINGSPELL_H
#define LABAOOP2_SUMMONINGSPELL_H

#include "SpellOnCoords.h"
#include <map>
#include "../../../../Factories/AllySpawner.h"

class SummoningSpell : public SpellOnCoords{
public:
    SummoningSpell(AllySpawner &spawner) : SpellOnCoords(SpellType::SummoningSpell), spawner(spawner) {}

    void castSpell(int gradeLevel, Field &field, Constants::XYPair from, Constants::XYPair to) const override;

    std::unique_ptr<Spell> clone() const override;

private:
    AllySpawner &spawner;
    std::map<int, int> levels = { //кол-во заспавненных союзников в зависимости от уровня
            {0, 1}, //0 - ослабленная вервсия заклинания для тестов
            {1, 1},
            {2, 1},
            {3, 2},
            {4, 3}
    };
};


#endif //LABAOOP2_SUMMONINGSPELL_H
