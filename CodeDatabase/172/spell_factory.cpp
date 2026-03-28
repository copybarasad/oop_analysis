#include "spell_factory.h"
#include "area_damage_spell.h"
#include "direct_damage_spell.h"
#include "trap_spell.h"

namespace game {
    std::unique_ptr<ISpell> SpellFactory::createRandom(std::mt19937& rng,
                                                       const GameConfig& cfg)
    {
        std::uniform_int_distribution<int> spellType(0, 6);

        const int pick = spellType(rng);

        switch (pick) {
            case 0:
            case 1:
            case 2: {
                const int dmg = cfg.playerSpellDirectDamage;
                const int r   = cfg.playerSpellDirectRadius;
                
                return std::make_unique<DirectDamageSpell>(dmg, r);
            }
            case 3:
            case 4: {
                const int dmg = cfg.playerSpellAreaDamage;
                const int r   = cfg.playerSpellAreaRadius;
                
                return std::make_unique<AreaDamageSpell>(dmg, r);
            }
            case 5:
            case 6: {
                const int dmg = cfg.trapSpellBaseDamage;

                return std::make_unique<TrapSpell>(dmg);
            }
            default: {
                return nullptr;
            }
        }
    }

    std::unique_ptr<ISpell> SpellFactory::createById(SpellId id, const GameConfig& cfg)
    {
        switch (id) {
            case SpellId::DirectDamage:
                return std::make_unique<DirectDamageSpell>(cfg.playerSpellDirectDamage, cfg.playerSpellDirectRadius);
            case SpellId::AreaDamage:
                return std::make_unique<AreaDamageSpell>(cfg.playerSpellAreaDamage, cfg.playerSpellAreaRadius);
            case SpellId::Trap:
                return std::make_unique<TrapSpell>(cfg.trapSpellBaseDamage);
        }

        return nullptr;
    }
}