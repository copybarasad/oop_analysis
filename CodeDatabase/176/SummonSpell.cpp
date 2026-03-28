#include "SummonSpell.h"
#include "GameManager.h"
#include <iostream>
#include <cmath>

SummonSpell::SummonSpell(int count, int spellRange, int cost)
    : summonCount(count), range(spellRange), manaCost(cost) {
    name = "Summon Ally";
    description = "Summons 1 ally near player";
}

bool SummonSpell::cast(GameManager& gameManager, int targetX, int targetY) {
    int summoned = gameManager.summonAlliesNearPlayer(summonCount);
    if (summoned > 0) {
        std::cout << "Summoned " << summoned << " allies near player!\n";
        return true;
    }
    std::cout << "No space to summon allies!\n";
    return false;
}

std::string SummonSpell::getName() const {
    return name;
}

std::string SummonSpell::getDescription() const {
    return description;
}

int SummonSpell::getManaCost() const {
    return manaCost;
}

bool SummonSpell::requiresTarget() const {
    return false;
}

int SummonSpell::getSummonCount() const {
    return summonCount;
}

int SummonSpell::getRange() const {
    return range;
}
