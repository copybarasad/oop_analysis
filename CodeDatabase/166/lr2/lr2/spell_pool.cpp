#include "spell_pool.hpp"

#include <array>
#include <random>

// конкретные типы карт
#include "spells/direct_damage_spell.hpp"
#include "spells/aoe_damage_spell.hpp"
#include "spells/trap_spell.hpp"
#include "spells/summon_spell.hpp"
#include "spells/enhance_spell.hpp"

namespace {
    std::unique_ptr<Spell> makeDirect() { return std::unique_ptr<Spell>(new DirectDamageSpell(4, 3)); }
    std::unique_ptr<Spell> makeAoE()    { return std::unique_ptr<Spell>(new AoEDamageSpell(3, 4)); }
    std::unique_ptr<Spell> makeTrap()   { return std::unique_ptr<Spell>(new TrapSpell(5)); }
    std::unique_ptr<Spell> makeSummon() { return std::unique_ptr<Spell>(new SummonSpell(1, 6, 2)); } // союзники hp=6
    std::unique_ptr<Spell> makeEnh()    { return std::unique_ptr<Spell>(new EnhanceSpell(1, 1, 2, 1)); }

    using MakerFn = std::unique_ptr<Spell>(*)();
    constexpr std::array<MakerFn, 5> POOL = { makeDirect, makeAoE, makeTrap, makeSummon, makeEnh };

    std::mt19937& rng() {
        static std::mt19937 gen{ std::random_device{}() };
        return gen;
    }
}

std::unique_ptr<Spell> makeRandomSpell() {
    std::uniform_int_distribution<int> dist(0, (int)POOL.size()-1);
    return POOL[(std::size_t)dist(rng())]();
}
