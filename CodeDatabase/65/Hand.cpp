#include "objects/Hand.h"
#include "Field.h"
#include "objects/Player.h"
#include "spells/SpellFactory.h"

bool Hand::addSpell(std::unique_ptr<ISpell> spell) {
    if (isFull()) return false;
    spells.push_back(std::move(spell));
    return true;
}

bool Hand::useSpell(int index, Field& field, Player* caster, int targetX, int targetY) {
    if (!isIndexInRange(index)) return false;
    bool result = spells[index]->use(field, caster, targetX, targetY);
    if (result) {
        removeSpell(index);
    }
    return result;
}

void Hand::removeSpell(int index) {
    if (isIndexInRange(index)) {
        spells.erase(spells.begin() + index);
    }
}

void Hand::addRandomSpell() {
    if (isFull()) return;
    spells.push_back(SpellFactory::createRandomSpell());
}
