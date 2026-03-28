#include "SpellFactory.h"
#include "SpellDirectDamage.h"
#include "SpellAreaDamage.h"
#include "SpellTrap.h"
#include "SpellSummon.h"
#include "SpellEnhance.h"
#include <cstdlib>

namespace SpellFactory {
    std::unique_ptr<ISpell> MakeDirect(int d, int r) { return std::make_unique<SpellDirectDamage>(d,r); }
    std::unique_ptr<ISpell> MakeArea(int d, int r, int s) { return std::make_unique<SpellAreaDamage>(d,r,s); }
    std::unique_ptr<ISpell> MakeTrap(int d) { return std::make_unique<SpellTrap>(d); }
    std::unique_ptr<ISpell> MakeSummon(int c) { return std::make_unique<SpellSummon>(c); }
    std::unique_ptr<ISpell> MakeEnhance(int r, int a, int t, int s) { return std::make_unique<SpellEnhance>(r,a,t,s); }

    std::unique_ptr<ISpell> MakeRandom() {
        switch (std::rand() % 5) {
            case 0: return MakeDirect();
            case 1: return MakeArea();
            case 2: return MakeTrap();
            case 3: return MakeSummon();
            default: return MakeEnhance();
        }
    }
}