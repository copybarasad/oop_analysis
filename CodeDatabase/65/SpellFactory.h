#pragma once

#include "interfaces/ISpell.h"
#include "spells/DirectDamageSpell.h"
#include "spells/AreaDamageSpell.h"
#include "spells/TrapSpell.h"
#include <memory>
#include <vector>
#include <random>

class SpellFactory {
public:
    static std::unique_ptr<ISpell> createSpell(SpellType type, int range, int damage, int areaSize) {
        switch (type) {
            case SpellType::DIRECT_DAMAGE:
                return std::make_unique<DirectDamageSpell>(range, damage);
            case SpellType::AREA_DAMAGE:
                return std::make_unique<AreaDamageSpell>(range, damage, areaSize);
            case SpellType::TRAP:
                return std::make_unique<TrapSpell>(range, damage, areaSize);
            default:
                return std::make_unique<DirectDamageSpell>(range, damage);
        }
    }

    static std::unique_ptr<ISpell> createRandomSpell() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, static_cast<int>(SpellType::SPELL_TYPE_COUNT) - 1);
        
        int spellType = dis(gen);
        
        switch (static_cast<SpellType>(spellType)) {
            case SpellType::DIRECT_DAMAGE:
                return std::make_unique<DirectDamageSpell>();
            case SpellType::AREA_DAMAGE:
                return std::make_unique<AreaDamageSpell>();
            case SpellType::TRAP:
                return std::make_unique<TrapSpell>();
            default:
                return std::make_unique<DirectDamageSpell>();
        }
    }

    static std::vector<std::unique_ptr<ISpell>> createThreeRandomSpells() {
        std::vector<std::unique_ptr<ISpell>> spells;
        for (int i = 0; i < 3; i++) {
            spells.push_back(createRandomSpell());
        }
        return spells;
    }
};
