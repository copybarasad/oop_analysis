#include "Spell.h"

Spell::Spell(const std::string& name, int mana_cost, int range, SpellType type)
    : name_(name),
      mana_cost_(mana_cost),
      range_(range),
      type_(type),
      level_(1) {}

Spell::~Spell() = default;

std::string Spell::GetName() const {
    return name_;
}

int Spell::GetManaCost() const {
    return mana_cost_;
}

int Spell::GetRange() const {
    return range_;
}

SpellType Spell::GetType() const {
    return type_;
}

int Spell::GetLevel() const {
    return level_;
}
