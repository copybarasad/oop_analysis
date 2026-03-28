#include "spell_pool.hpp"

#include <random>

#include "spells/direct_damage_spell.hpp"
#include "spells/aoe_damage_spell.hpp"
#include "spells/trap_spell.hpp"
#include "spells/summon_spell.hpp"
#include "spells/enhance_spell.hpp"

std::unique_ptr<Spell> makeRandomSpell() {
    static std::mt19937 gen{ std::random_device{}() };
    // Пул без башни: { direct, aoe, trap, summon, enhance }
    std::uniform_int_distribution<int> dist(0, 4);
    switch (dist(gen)) {
        case 0: return std::unique_ptr<Spell>(new DirectDamageSpell(4, 3));
        case 1: return std::unique_ptr<Spell>(new AoEDamageSpell(3, 4));
        case 2: return std::unique_ptr<Spell>(new TrapSpell(4));
        case 3: return std::unique_ptr<Spell>(new SummonSpell(1, 5, 2));
        default:return std::unique_ptr<Spell>(new EnhanceSpell());
    }
}
