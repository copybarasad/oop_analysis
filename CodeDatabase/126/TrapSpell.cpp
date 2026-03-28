#include "../../headers/spells/TrapSpell.h"
#include "../../headers/objects/Trap.h"
#include "../../headers/field/GameField.h"
#include "../../headers/objects/GameObject.h"
#include <iostream>
#include <sstream>

using namespace std;

TrapSpell::TrapSpell(int dmg, int rng, string spellName) : damage(dmg), range(rng), name(spellName) {}

string TrapSpell::getName() const { return name; }

string TrapSpell::getDescription() const { 
    return "Places a trap that deals " + to_string(damage) + " damage when stepped on";
}

int TrapSpell::getRange() const { return range; }

bool TrapSpell::canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const {
    if (!field.isValidPosition(targetPos)) return false;
    if (field.isCellBlocked(targetPos)) return false;
    double distance = casterPos.distanceTo(targetPos);
    return distance <= range && distance > 0 && field.isCellFree(targetPos);
}

void TrapSpell::cast(const Position& casterPos, const Position& targetPos, GameField& field, vector<shared_ptr<GameObject>>& objects) {
    auto trap = make_shared<Trap>(targetPos, damage);
    objects.push_back(trap);
}

string TrapSpell::serialize() const {
    return "TrapSpell " + to_string(damage) + " " + to_string(range) + " " + name;
}