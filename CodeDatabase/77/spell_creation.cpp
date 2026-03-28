#include "spell_creation.h"

#include <random>


std::unique_ptr<ISpell> createRandomSpell()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 2);

    int randomSpellType = distrib(gen);

    switch (randomSpellType)
    {
    case 0:
        return std::make_unique<SingleTarget>();
    case 1:
        return std::make_unique<AreaDamage>();
    case 2:
    default:
        return std::make_unique<Traps>();
    }
}