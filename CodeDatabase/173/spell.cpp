#include <stdexcept>

#include "spell.hpp"

#include "aoe-spell.hpp"
#include "direct-damage-spell.hpp"
#include "summon-spell.hpp"
#include "trap-spell.hpp"
#include "upgrade-spell.hpp"

Spell *Spell::fromByteDump(bytedump bytes) {
    if (bytes.size() != 4) {
        throw std::length_error("spell dump must be 4 bytes long");
    }

    int spellType = static_cast<int>(bytes[0]);

    switch (spellType) {
    case AOE_SPELL_BYTE:
        return new AoESpell(bytes);
    case DIRECT_SPELL_BYTE:
        return new DirectDamageSpell(bytes);
    case SUMMON_SPELL_BYTE:
        return new SummonSpell(bytes);
    case TRAP_SPELL_BYTE:
        return new TrapSpell(bytes);
    case UPGRADE_SPELL_BYTE:
        return new UpgradeSpell(bytes);
    default:
        throw std::logic_error("unknown spell type");
    }
}
