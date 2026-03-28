#include "RangedSpell.hpp"

RangedSpell::RangedSpell (Field& field, size_t range, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target) :
    field(field), range(range), casterX(caster.first), casterY(caster.second), targetX(target.first), targetY(target.second) {}

size_t RangedSpell::getRange () const { return range; }

void RangedSpell::setCaster (std::pair<size_t, size_t> caster) { casterX = caster.first; casterY = caster.second; }

void RangedSpell::setTarget (std::pair<size_t, size_t> target) { targetX = target.first; targetY = target.second; }

size_t RangedSpell::calculateDistance (size_t x, size_t y) const {
    return std::sqrt(std::pow((int)casterX - (int)x, 2) + std::pow((int)casterY - (int)y, 2));
}