#include "BuffSpell.hpp"

BuffSpell::BuffSpell (SpellHand& hand, size_t level) : hand(hand), level(level) {}

Spell::CastResult BuffSpell::cast () {
    CastResult result;
    result.success = true;
    level++;
    result.level = level;
    hand.upgradeHand(level);
    return result;
}

void BuffSpell::upgradeSpell (size_t level) { this->level += level; }