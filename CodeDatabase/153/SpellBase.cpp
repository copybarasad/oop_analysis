#include "SpellBase.h"
#include "SpellTarget.h"

SpellBase::SpellBase(const std::string& spellName, int cost, int spellRange, int spellDamage)
    : name(spellName), manaCost(cost), range(spellRange), damage(spellDamage) {
}

bool SpellBase::validateTarget(const SpellTarget& target, const GameField& field) const {
    int distance = std::abs(target.targetX - target.casterX) +
        std::abs(target.targetY - target.casterY);

    if (distance > range) {
        std::cout << "Target is too far! Distance: " << distance
            << ", max: " << range << std::endl;
        return false;
    }

    if (!field.isValidPosition(target.targetX, target.targetY)) {
        std::cout << "Invalid target position!" << std::endl;
        return false;
    }

    return true;
}

std::string SpellBase::getName() const {
    return name;
}

int SpellBase::getManaCost() const {
    return manaCost;
}

int SpellBase::getRange() const {
    return range;
}