#pragma once
#include <memory>
#include "ISpell.h"

namespace SpellFactory {
    std::unique_ptr<ISpell> MakeDirect(int dmg = 6, int radius = 3);
    std::unique_ptr<ISpell> MakeArea(int dmg = 3, int radius = 4, int size = 2);
    std::unique_ptr<ISpell> MakeTrap(int dmg = 5);
    std::unique_ptr<ISpell> MakeSummon(int count = 1);
    std::unique_ptr<ISpell> MakeEnhance(int r = 1, int a = 1, int t = 2, int s = 1);

    std::unique_ptr<ISpell> MakeRandom();
}