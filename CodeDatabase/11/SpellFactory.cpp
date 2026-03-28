#include "SpellFactory.h"
#include "FireballSpell.h"
#include "ZapSpell.h"
#include "BuffSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include <random>

std::unique_ptr<ISpell> SpellFactory::createRandomSpell() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 4);

    switch (distrib(gen)) {
        case 0: return std::make_unique<FireballSpell>();
        case 1: return std::make_unique<ZapSpell>();
        case 2: return std::make_unique<TrapSpell>();
        case 3: return std::make_unique<SummonSpell>();
        case 4: default: return std::make_unique<BuffSpell>();
    }
}