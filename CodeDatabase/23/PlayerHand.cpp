#include "PlayerHand.h"
#include "Spell.h"
#include "Constants.h"
#include <random>

Spell* makeDirectSpell(int damage = DIRECT_SPELL_DAMAGE, int range = DIRECT_SPELL_RANGE);
Spell* makeAreaSpell();
Spell* makeTrapSpell();

PlayerHand::PlayerHand(int capacity) : capacity(capacity), rng(std::random_device{}()) {
}

PlayerHand::~PlayerHand() {
    for (auto s : spells) delete s;
}

int PlayerHand::size() const { return (int)spells.size(); }
int PlayerHand::getCapacity() const { return capacity; }
const std::vector<Spell*>& PlayerHand::getSpells() const { return spells; }

bool PlayerHand::addSpell(Spell* spell) {
    if ((int)spells.size() >= capacity) { delete spell; return false; }
    spells.push_back(spell);
    return true;
}

bool PlayerHand::addRandomSpell() {
    if ((int)spells.size() >= capacity) return false;
    std::uniform_int_distribution<int> dist(0, 2);
    int t = dist(rng);
    Spell* s = nullptr;
    if (t == 0) s = makeDirectSpell();
    else if (t == 1) s = makeAreaSpell();
    else s = makeTrapSpell();
    return addSpell(s);
}

bool PlayerHand::castAt(GameField& field, Player& player, int index, int targetX, int targetY) {
    if (index < 0 || index >= (int)spells.size()) return false;
    Spell* s = spells[index];
    bool used = s->cast(field, player, targetX, targetY);
    if (used) {
        delete s;
        spells.erase(spells.begin() + index);
    }
    return used;
}

void PlayerHand::clear() {
    for (auto s : spells) {
        delete s;
    }
    spells.clear();
}

void PlayerHand::setCapacity(int newCapacity) {
    if (newCapacity < 1) newCapacity = 1;
    capacity = newCapacity;
    while ((int)spells.size() > capacity) {
        delete spells.back();
        spells.pop_back();
    }
}

void PlayerHand::removeSpell(int index) {
    if (index < 0 || index >= (int)spells.size()) return;
    delete spells[index];
    spells.erase(spells.begin() + index);
}

std::vector<Spell*>& PlayerHand::getSpellsMutable() {
    return spells;
}


