#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "ImproveSpell.h"
#include "SummonSpell.h"
#include <memory>

std::unique_ptr<ISpell> SpellFactory::createSpell(SpellType type) {
    switch (type) {
        case SpellType::DIRECT_DAMAGE:
            return std::make_unique<DirectDamageSpell>();
        case SpellType::AREA_DAMAGE:
            return std::make_unique<AreaDamageSpell>();
        case SpellType::TRAP:
            return std::make_unique<TrapSpell>();
        case SpellType::IMPROVE:
            return std::make_unique<ImproveSpell>();
        case SpellType::SUMMON:
            return std::make_unique<SummonSpell>();
        default:
            return nullptr;
    }
}



