#include "Spell.h"

Spell::Spell(int id, const std::string& name, int cost)
    : id(id), name(name), cost(cost) {}

int Spell::getId() const { return id; }
std::string Spell::getName() const { return name; }
int Spell::getCost() const { return cost; }