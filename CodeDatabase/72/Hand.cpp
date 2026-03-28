//
// Created by Mac on 25.10.2025.
//

#include "Hand.h"

#include <iostream>

#include "../spells/AreaDamageSpell.h"
#include "../spells/DirectDamageSpell.h"
#include "../spells/SpellFactory.h"
#include "../spells/TrapSpell.h"

Hand::Hand(const int & maxSpells)
    :maxSize(maxSpells) {

    if (maxSpells <= 0)
        this->maxSize = DEFAULT_MAXSIZE;

    addRandomSpell();
}

void Hand::addRandomSpell() {
    int r = rand() % 3;
    switch (r) {
        case 0:
            spells.push_back(std::make_shared<DirectDamageSpell>());
            break;
        case 1:
            spells.push_back(std::make_shared<AreaDamageSpell>());
            break;
        case 2:
            spells.push_back(std::make_shared<TrapSpell>());
            break;
    }
}

void Hand::showHand() const {
    if (spells.empty()) {
        std::cout << "Your hand is empty!\n";
        return;
    }

    for (size_t i = 0; i < spells.size(); ++i) {
        if (spells[i])
            std::cout << i+1 << ". " << spells[i]->getName() << std::endl;
        else
            std::cout << i+1 << ". [invalid spell]" << std::endl;
    }
}

void Hand::addSpell(const std::shared_ptr<ISpell> & spell) {
    this->spells.push_back(spell);
}

std::shared_ptr<ISpell> Hand::getSpell(size_t index) {
    std::shared_ptr<ISpell> spell = spells.at(index);
    spells.erase(spells.begin() + index);
    return spell;
}

size_t Hand::getCount() const {
    return this->spells.size();
}

bool Hand::canAddSpell() const {
    return !(this->spells.size() == this->maxSize);
}

void Hand::removeHalfRandom() {
    if (spells.empty()) return;

    size_t countToRemove = spells.size() / 2;

    for (size_t i = 0; i < countToRemove; ++i) {
        if (spells.empty()) break;

        size_t index = rand() % spells.size();
        spells.erase(spells.begin() + index);
    }
}

void Hand::save(Saver &saver) const {
    saver.save(maxSize);

    int count = (int)spells.size();
    saver.save(count);

    for (auto & spell: spells) {
        int typeInt = (int)spell->getType();
        saver.save(typeInt);
        spell->save(saver);
    }
}

void Hand::load(Reader &reader) {
    reader.read(maxSize);
    int count;
    reader.read(count);

    spells.clear();

    for (int i = 0; i < count; i++) {
        int typeInt;
        reader.read(typeInt);
        Type type = static_cast<Type>(typeInt);
        auto spell = SpellFactory::create(type);
        spell->load(reader);
        spells.push_back(spell);
    }
}









