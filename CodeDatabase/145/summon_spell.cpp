#include "summon_spell.h"
#include "summon_context.h"
#include "player.h"
#include "game_field.h"
#include "enemy.h"
#include "ally.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream> 

SummonSpell::SummonSpell(int summonCount, int health, int damage) 
    : summonCount(summonCount), summonHealth(health), summonDamage(damage) {}

bool SummonSpell::apply(const SummonContext& context) {
    if (!canUse(context)) {
        return false;
    }
    auto playerPos = context.player.getPosition();
    std::vector<std::pair<int, int>> positions = {
        {playerPos.first - 1, playerPos.second},
        {playerPos.first + 1, playerPos.second},
        {playerPos.first, playerPos.second - 1},
        {playerPos.first, playerPos.second + 1}
    };
    int summoned = 0;
    for (const auto& pos : positions) {
        if (summoned >= summonCount) break;
        if (context.field.isValidPosition(pos.first, pos.second) && 
            context.field.isPassable(pos.first, pos.second)) {
            if (!context.field.isOccupied(pos.first, pos.second, context.enemies, context.buildings, context.towers, &context.player, &context.allies)) {
                context.allies.push_back(Ally(summonHealth, summonDamage, pos.first, pos.second));
                std::cout << "Summoned ally at (" << pos.first << "," << pos.second << ")\n";
                summoned++;
            }
        }
    }
    std::cout << "Summon spell! Created " << summoned << " ally/allies.\n";
    return summoned > 0;
}

bool SummonSpell::canUse(const SummonContext& context) const {
    return context.field.isValidPosition(context.targetX, context.targetY) && context.field.isPassable(context.targetX, context.targetY);
}

const char* SummonSpell::getName() const {
    return "Summon";
}

std::unique_ptr<Spell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(summonCount, summonHealth, summonDamage);
}

int SummonSpell::getSummonCount() const {
    return summonCount;
}

void SummonSpell::setSummonCount(int newCount) {
    summonCount = newCount;
}

void SummonSpell::applyEnhancement() {
    setSummonCount(getSummonCount() + 1);
    std::cout << "  [Enhanced] Summon count increased to " << getSummonCount() << "\n";
}

std::string SummonSpell::getSerializedData() const {
    std::ostringstream oss;
    oss << "Summon " << summonCount;
    return oss.str();
}