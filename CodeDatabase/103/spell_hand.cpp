#include "spell_hand.h"
#include <stdexcept>
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"

SpellHand::SpellHand(int size) : maxSize(size) {}
SpellHand::~SpellHand() { for (auto* s : spells) delete s; }

void SpellHand::addSpell(ISpell* spell) {
    if (spells.size() < maxSize) spells.push_back(spell);
    else {
        std::cout << "Inventory is fully, the spell will not be added \n";
    }
}

const std::vector<ISpell*>& SpellHand::getSpells() const { return spells; }

int SpellHand::getMaxSize() const { return maxSize;}

ISpell* SpellHand::getSpell(int index) const {
    if (index >= 0 && index < spells.size()) return spells[index];
    return nullptr;
}

std::vector<std::tuple<std::string, int, int, int>> SpellHand::getSpellsData() const {
    std::vector<std::tuple<std::string, int, int, int>> data;
    for (const auto* spell : spells) {
        if (spell) {
            data.emplace_back(spell->getName(), spell->getDamage(), spell->getCost(), spell->getRadius());
        }
    }
    return data;
}

void SpellHand::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        delete spells[index];
        spells.erase(spells.begin() + index);
    }
}

void SpellHand::addSpellByName(const std::string& name, int damage,  int cost, int radius) {
    if (spells.size() >= maxSize) return;
    if (damage < 1 || cost < 1 || radius < 1){
        throw std::invalid_argument("Incorrect spell params");
    }

    ISpell* newSpell = nullptr;
    if (name == "Direct_Damage") {
        newSpell = new DirectDamageSpell(damage, cost, radius);
    }
    else if (name == "Area_Damage") {
        newSpell = new AreaDamageSpell(damage, cost, radius);
    }
    else if (name == "Trap") {
        newSpell = new TrapSpell(damage, cost, radius);
    }else{
        throw std::invalid_argument("Incorrect spell name");
    }
    
    if (newSpell) {
        spells.push_back(newSpell);
    }
}