#include "../include/SpellHand.h"
#include "../include/DirectDamageSpell.h"
#include "../include/AreaDamageSpell.h"
#include "../include/TrapSpell.h"
#include "../include/SummonSpell.h"
#include "../include/EnhancementSpell.h"
#include <cstdlib>
#include <ctime>

SpellHand::SpellHand(int maxSpells) : maxSpells(maxSpells) {
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(nullptr)) + 42);
        initialized = true;
    }
}

void SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (static_cast<int>(spells.size()) < maxSpells) {
        spells.push_back(std::move(spell));
    }
}

void SpellHand::removeSpell(int index) {
    if (hasSpell(index)) {
        spells.erase(spells.begin() + index);
    }
}

bool SpellHand::hasSpell(int index) const {
    return index >= 0 && index < static_cast<int>(spells.size());
}

Spell* SpellHand::getSpell(int index) {
    if (hasSpell(index)) {
        return spells[index].get();
    }
    return nullptr;
}

const Spell* SpellHand::getSpell(int index) const {
    if (hasSpell(index)) {
        return spells[index].get();
    }
    return nullptr;
}

int SpellHand::getSpellCount() const {
    return static_cast<int>(spells.size());
}

int SpellHand::getMaxSpells() const {
    return maxSpells;
}

void SpellHand::addRandomSpell() {
    if (static_cast<int>(spells.size()) >= maxSpells) {
        return;
    }
    
    int spellType = rand() % 5;
    
    switch (spellType) {
        case 0:
            addSpell(std::make_unique<DirectDamageSpell>());
            break;
        case 1:
            addSpell(std::make_unique<AreaDamageSpell>());
            break;
        case 2:
            addSpell(std::make_unique<TrapSpell>());
            break;
        case 3:
            addSpell(std::make_unique<SummonSpell>());
            break;
        case 4:
            addSpell(std::make_unique<EnhancementSpell>());
            break;
    }
}

const std::vector<std::unique_ptr<Spell>>& SpellHand::getSpells() const {
    return spells;
}
