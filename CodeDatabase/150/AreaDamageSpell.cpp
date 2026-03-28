#include "AreaDamageSpell.hpp"
#include "GameState.hpp"
#include "SpellCaster.hpp"
#include "EntityManager.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize, int manaCost)
    : damage_(damage), range_(range), manaCost_(manaCost), type_(SpellType::AreaDamage), areaSize_(areaSize) {}

bool AreaDamageSpell::cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) {
    int casterX = caster.getPositionX();
    int casterY = caster.getPositionY();
    
    if (!checkRange(casterX, casterY, targetX, targetY)) {
        return false;
    }
    
    for (int x = targetX - areaSize_/2; x <= targetX + areaSize_/2; ++x) {
        for (int y = targetY - areaSize_/2; y <= targetY + areaSize_/2; ++y) {
            if (x == casterX && y == casterY) {
                continue;
            }
            
            if (gameState.field.isValidPosition(x, y)) {
                EntityManager::damageEnemyAt(gameState, x, y, damage_);
                EntityManager::damageAllyAt(gameState, x, y, damage_);
                EntityManager::damageBuildingAt(gameState, x, y, damage_);
            }
        }
    }
    
    return true;
}

std::unique_ptr<ISpell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(*this);
}

bool AreaDamageSpell::checkRange(int casterX, int casterY, int targetX, int targetY) const {
    int distance = std::abs(casterX - targetX) + std::abs(casterY - targetY);
    return distance <= range_;
}

void AreaDamageSpell::enhanceArea(int bonus) {
    areaSize_++;
}

void AreaDamageSpell::applyEnhancements(int enhancementCount) {
    enhanceArea(enhancementCount);
}

void AreaDamageSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&damage_), sizeof(damage_));
    os.write(reinterpret_cast<const char*>(&range_), sizeof(range_));
    os.write(reinterpret_cast<const char*>(&manaCost_), sizeof(manaCost_));
    os.write(reinterpret_cast<const char*>(&areaSize_), sizeof(areaSize_));
}

void AreaDamageSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&damage_), sizeof(damage_));
    is.read(reinterpret_cast<char*>(&range_), sizeof(range_));
    is.read(reinterpret_cast<char*>(&manaCost_), sizeof(manaCost_));
    is.read(reinterpret_cast<char*>(&areaSize_), sizeof(areaSize_));
}