//
// Created by Mac on 11.11.2025.
//

#include "SpellFactory.h"

#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"


std::shared_ptr<ISpell> SpellFactory::create(const Type &type) {
    switch (type) {
        case Type::DirectDamage: return std::make_shared<DirectDamageSpell>();
        case Type::AreaDamage:   return std::make_shared<AreaDamageSpell>();
        case Type::Trap:         return std::make_shared<TrapSpell>();
        default: return nullptr;
    }
}
