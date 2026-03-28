#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhanceSpell.h"
#include "Enhancement.h"
#include <chrono>
#include <stdexcept>

SpellFactory::SpellFactory()
    : randomGenerator(std::chrono::steady_clock::now().time_since_epoch().count()) {
}

Spell *SpellFactory::createRandomSpell() {
    std::uniform_int_distribution<int> distribution(0, 4);
    SpellType spellType = static_cast<SpellType>(distribution(randomGenerator));
    return createSpell(spellType);
}

Spell *SpellFactory::createSpell(SpellType type) {
    switch (type) {
        case SpellType::DIRECT_DAMAGE:
            return new DirectDamageSpell();
        case SpellType::AREA_DAMAGE:
            return new AreaDamageSpell();
        case SpellType::TRAP:
            return new TrapSpell();
        case SpellType::SUMMON:
            return new SummonSpell();
        case SpellType::ENHANCE: {
            EnhancementType types[] = {
                EnhancementType::INCREASE_RANGE,
                EnhancementType::INCREASE_AREA,
                EnhancementType::INCREASE_DAMAGE,
                EnhancementType::INCREASE_SUMMON_COUNT
            };
            std::uniform_int_distribution<int> enhancementDist(0, 3);
            Enhancement randomEnhancement = {types[enhancementDist(randomGenerator)], 1};
            return new EnhanceSpell(randomEnhancement);
        }
    }
    return new DirectDamageSpell();
}

Spell *SpellFactory::createSpell(SpellType type, std::istream &in) {
    switch (type) {
        case SpellType::DIRECT_DAMAGE: {
            int damage, range;
            if (!(in >> damage >> range)) {
                throw std::runtime_error("Ошибка чтения параметров прямого урона");
            }
            return new DirectDamageSpell(damage, range);
        }
        case SpellType::AREA_DAMAGE: {
            int damage, range, area;
            if (!(in >> damage >> range >> area)) {
                throw std::runtime_error("Ошибка чтения параметров урона по области");
            }
            AreaDamageSpell *spell = new AreaDamageSpell(damage, range);
            Enhancement enh{EnhancementType::INCREASE_AREA, area - 2};
            if (area > 2) spell->enhance(enh);
            return spell;
        }
        case SpellType::TRAP: {
            int damage, range;
            if (!(in >> damage >> range)) {
                throw std::runtime_error("Ошибка чтения параметров ловушки");
            }
            return new TrapSpell(damage, range);
        }
        case SpellType::SUMMON: {
            int count;
            if (!(in >> count)) {
                throw std::runtime_error("Ошибка чтения параметров призыва");
            }
            return new SummonSpell(count);
        }
        case SpellType::ENHANCE: {
            int enhType, value;
            if (!(in >> enhType >> value)) {
                throw std::runtime_error("Ошибка чтения параметров улучшения");
            }
            Enhancement enh = {static_cast<EnhancementType>(enhType), value};
            return new EnhanceSpell(enh);
        }
    }
    throw std::runtime_error("Неизвестный тип заклинания");
}
