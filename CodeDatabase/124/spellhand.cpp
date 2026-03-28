#include "spellhand.h"
#include "spellcard.h"  
#include <iostream>
#include <random>
#include <chrono>
SpellHand::SpellHand(int maxSpells) : maxSize(maxSpells), selectedIndex(0) { 
    std::random_device rd;
    auto time_seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    auto combined_seed = rd() ^ time_seed;
    rng.seed(combined_seed);
    
}


bool SpellHand::addSpell(std::shared_ptr<SpellCard> spell) {
    if (spells.size() >= maxSize) {
        return false;
    }
    spells.push_back(spell);
    return true;
}

void SpellHand::selectNext() {
    if (spells.empty()) return;
    selectedIndex = (selectedIndex + 1) % spells.size();
}

void SpellHand::selectPrevious() {
    if (spells.empty()) return;
    selectedIndex = (selectedIndex - 1 + spells.size()) % spells.size();
}

std::shared_ptr<SpellCard> SpellHand::getSelectedSpell() {
    if (spells.empty()) return nullptr;
    return spells[selectedIndex];
}

const std::vector<std::shared_ptr<SpellCard>>& SpellHand::getSpells() const {
    return spells;
}

int SpellHand::getSelectedIndex() const {
    return selectedIndex;
}

int SpellHand::getSize() const {
    return spells.size();
}

int SpellHand::getMaxSize() const {
    return maxSize;
}

bool SpellHand::isFull() const {
    return spells.size() >= maxSize;
}

std::shared_ptr<SpellCard> SpellHand::generateRandomSpell() {
    std::uniform_int_distribution<int> typeDist(0, 3);
    int randomType = typeDist(rng);    
    switch (randomType) {
        case 0: return std::make_shared<FireballSpell>();
        case 1: return std::make_shared<MagicArrowSpell>();
        case 2: return std::make_shared<IceStormSpell>();
        case 3: return std::make_shared<MagicTrapSpell>();
        default: return std::make_shared<FireballSpell>();
    }
}
 

bool SpellHand::addRandomSpell() {
    if (isFull()) return false;
    return addSpell(generateRandomSpell());
}

void SpellHand::updateCooldowns() {
    for (auto& spell : spells) {
        spell->updateCooldown();
    }
}

const std::shared_ptr<SpellCard> SpellHand::getSelectedSpell() const {
    if (spells.empty()) return nullptr;
    return spells[selectedIndex];
}



std::vector<std::string> SpellHand::getSpellNames() const {
    std::vector<std::string> names;
    for (const auto& spell : spells) {
        if (spell) {
            names.push_back(spell->getName());
        }
    }
    return names;
}
 
void SpellHand::copySpellsFrom(const SpellHand& other) {
    spells.clear();
    for (const auto& spell : other.spells) {
        spells.push_back(spell);
    }
    selectedIndex = 0;
}
 
bool SpellHand::removeRandomSpell() {
    if (spells.empty()) {
        return false;
    }
    
    std::uniform_int_distribution<int> dist(0, spells.size() - 1);
    int randomIndex = dist(rng);
    
    spells.erase(spells.begin() + randomIndex);
     
    if (selectedIndex >= spells.size()) {
        selectedIndex = spells.size() - 1;
        if (selectedIndex < 0) selectedIndex = 0;
    }
    
    return true;
}

void SpellHand::clear() {
    spells.clear();
    selectedIndex = 0;
}
bool SpellHand::addSpellByName(const std::string& spellName) {
    
    if (spells.size() >= maxSize) {
        std::cout << "  ❌ Рука заполнена!" << std::endl;
        return false;
    }
    
    std::shared_ptr<SpellCard> spell;
    
    if (spellName == "Магическая стрела") {
        spell = std::make_shared<MagicArrowSpell>();
        std::cout << "  ✅ Создана Магическая стрела" << std::endl;
    }
    else if (spellName == "Огненный шар") {
        spell = std::make_shared<FireballSpell>();
        std::cout << "  ✅ Создан Огненный шар" << std::endl;
    }
    else if (spellName == "Ледяная буря") {
        spell = std::make_shared<IceStormSpell>();
        std::cout << "  ✅ Создана Ледяная буря" << std::endl;
    }
    else if (spellName == "Магическая ловушка") {
        spell = std::make_shared<MagicTrapSpell>();
        std::cout << "  ✅ Создана Магическая ловушка" << std::endl;
    }
    else {
        std::cout << "  ❌ Неизвестное заклинание: '" << spellName << "'" << std::endl;
    }
    
    if (spell) {
        spells.push_back(spell);
        std::cout << "  ✅ Заклинание добавлено в руку" << std::endl;
        return true;
    }
    
    return false;
}