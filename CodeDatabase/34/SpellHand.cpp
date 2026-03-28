#include "SpellHand.h"
#include <utility>
#include <algorithm>
#include <random>

#include "utils/RandomGenerator.h"
#include "spells/DirectDamageSpell.h"
#include "spells/AreaDamageSpell.h"
#include "spells/TrapSpell.h"
#include "spells/SummonSpell.h"
#include "spells/EnhancementSpell.h"
#include "Exceptions.h"

namespace Game {

SpellHand::SpellHand(size_t maxSize) : maxSize(maxSize) {}

bool SpellHand::addSpell(std::unique_ptr<ISpell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

ISpell* SpellHand::getSpell(size_t index) {
    if (index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

const ISpell* SpellHand::getSpell(size_t index) const {
    if (index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

const std::vector<std::unique_ptr<ISpell>>& SpellHand::getAllSpells() const {
    return spells;
}

size_t SpellHand::getSpellCount() const {
    return spells.size();
}

size_t SpellHand::getMaxSize() const {
    return maxSize;
}

std::unique_ptr<ISpell> SpellHand::useSpell(size_t index) {
    if (index < spells.size()) {
        std::unique_ptr<ISpell> spellToUse = std::move(spells[index]);
        spells.erase(spells.begin() + index);
        return spellToUse;
    }
    return nullptr;
}

void SpellHand::removeRandomSpells(size_t count) {
    if (count == 0) return;
    if (count >= spells.size()) {
        spells.clear();
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(spells.begin(), spells.end(), g);

    spells.erase(spells.begin(), spells.begin() + count);
}

void SpellHand::serialize(std::ostream& os) const {
    size_t spellCount = spells.size();
    os.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    for (const auto& spell : spells) {
        SpellType type = spell->getType();
        os.write(reinterpret_cast<const char*>(&type), sizeof(type));
        spell->serialize(os);
    }
}

void SpellHand::deserialize(std::istream& is) {
    spells.clear();
    size_t spellCount;
    is.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    if (is.fail()) throw FileReadException("Failed to read spell count.");
    if (spellCount > 100) throw InvalidDataException("Unrealistic spell count in save file.");


    for (size_t i = 0; i < spellCount; ++i) {
        SpellType type;
        is.read(reinterpret_cast<char*>(&type), sizeof(type));
        if (is.fail()) throw FileReadException("Failed to read spell type.");

        std::unique_ptr<ISpell> newSpell;
        switch (type) {
            case SpellType::DirectDamage: newSpell = std::make_unique<DirectDamageSpell>(); break;
            case SpellType::AreaDamage:   newSpell = std::make_unique<AreaDamageSpell>(); break;
            case SpellType::Trap:         newSpell = std::make_unique<TrapSpell>(); break;
            case SpellType::Summon:       newSpell = std::make_unique<SummonSpell>(); break;
            case SpellType::Enhancement:  newSpell = std::make_unique<EnhancementSpell>(); break;
            default: throw InvalidDataException("Unknown spell type found in save file.");
        }
        newSpell->deserialize(is);
        if(is.fail()) throw FileReadException("Failed to deserialize spell data.");
        spells.push_back(std::move(newSpell));
    }
}

std::unique_ptr<ISpell> SpellHand::createRandomSpell() {
    Utils::RandomGenerator& rng = Utils::RandomGenerator::getInstance();
    int spellType = rng.getRandomInt(0, 4);
    std::unique_ptr<ISpell> newSpell;
    switch (spellType) {
        case 0: newSpell = std::make_unique<DirectDamageSpell>(); break;
        case 1: newSpell = std::make_unique<AreaDamageSpell>(); break;
        case 2: newSpell = std::make_unique<TrapSpell>(); break;
        case 3: newSpell = std::make_unique<SummonSpell>(); break;
        case 4: newSpell = std::make_unique<EnhancementSpell>(); break;
    }
    // Уведомление о получении заклинания теперь происходит через систему событий
    return newSpell;
}

}