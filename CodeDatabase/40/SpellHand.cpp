#include "SpellHand.h"
#include "fireball.h"
#include "BOMB.h"
#include <stdexcept>
#include <iostream>

SpellHand::SpellHand(size_t maxSpells) : maxSize(maxSpells){
    addSpell(std::make_unique<Fireball>(1, "Fireball", 0, 10, 3));
    addSpell(std::make_unique<BOMB>(2, "BOMB", 0, 50, 3));
}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell){
    if (spells.size() >= maxSize || hasSpell(spell->getId())){
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

const Spell* SpellHand::getSpell(size_t index) const {
    if (index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

size_t SpellHand::size() const {
    return spells.size();
}

bool SpellHand::hasSpell(int spellId) const{
    for (const auto& spell : spells) {
        if (spell->getId() == spellId) {
            return true;
        }
    }
    return false;
}

void SpellHand::save(std::ostream& out) const {
    size_t numSpells = spells.size();
    out.write(reinterpret_cast<const char*>(&numSpells), sizeof(numSpells));
    for (const auto& spell : spells) {
        int id = spell->getId();
        size_t nameLen = spell->getName().length();
        int cost = spell->getCost();
        bool isArea = spell->isAreaSpell(); 

        out.write(reinterpret_cast<char*>(&id), sizeof(id));
        out.write(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        out.write(spell->getName().c_str(), nameLen);
        out.write(reinterpret_cast<char*>(&cost), sizeof(cost));
        out.write(reinterpret_cast<char*>(&isArea), sizeof(isArea));
    }
}

void SpellHand::load(std::istream& in){
    size_t numSpells;
    in.read(reinterpret_cast<char*>(&numSpells), sizeof(numSpells));
    
    
    spells.clear();
    for (size_t i = 0; i < numSpells; ++i){
        int id, cost;
        size_t nameLen;
        bool isArea;

        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        std::string name(nameLen, '\0');
        in.read(&name[0], nameLen);
        in.read(reinterpret_cast<char*>(&cost), sizeof(cost));
        in.read(reinterpret_cast<char*>(&isArea), sizeof(isArea));
        
        std::unique_ptr<Spell> loadedSpell;
        if (isArea){
            loadedSpell = std::make_unique<BOMB>(id, name, cost, 50, 3);
        } else {
            loadedSpell = std::make_unique<Fireball>(id, name, cost, 10, 3);
        }
        spells.push_back(std::move(loadedSpell));
    }
}