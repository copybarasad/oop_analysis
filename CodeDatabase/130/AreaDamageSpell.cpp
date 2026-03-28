#include "AreaDamageSpell.h"
#include "IGameWorldFacade.h"
#include <iostream>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize, const std::string& name)
    : baseDamage_(damage), baseRange_(range), baseAreaSize_(areaSize), name_(name) {}

bool AreaDamageSpell::cast(SpellContext& context, EnhancementState& enhancements) {
    if (!context.world) {
        return false;
    }
    
    Pos playerPos = context.world->getPlayerPosition();
    Pos target = context.targetPosition;

    int effectiveRange = baseRange_ + enhancements.rangeBonus;
    int effectiveAreaSize = baseAreaSize_ + enhancements.areaBonus;
    if (effectiveAreaSize < 1) effectiveAreaSize = 1;
    int effectiveDamage = baseDamage_ + enhancements.damageBonus;

    if (!context.world->isInRange(playerPos, target, effectiveRange)) {
        std::cout << "Target out of range! Max range: " << effectiveRange << "\n";
        return false;
    }
    
    std::cout << "Cast " << name_ << "! Area: " << effectiveAreaSize << "x" << effectiveAreaSize << "\n";
    
    int damageDealt = 0;
    for (int dy = 0; dy < effectiveAreaSize; ++dy) {
        for (int dx = 0; dx < effectiveAreaSize; ++dx) {
            int targetX = target.x + dx;
            int targetY = target.y + dy;
            
            if (!context.world->isValidPosition({targetX, targetY})) {
                continue;
            }

            if (context.world->dealDamageToEnemy({targetX, targetY}, effectiveDamage)) {
                damageDealt++;
            }
            if (context.world->dealDamageToBuilding({targetX, targetY}, effectiveDamage)) {
                damageDealt++;
            }
            if (context.world->dealDamageToTower({targetX, targetY}, effectiveDamage)) {
                damageDealt++;
            }
        }
    }
    
    if (damageDealt > 0) {
        std::cout << "Damaged " << damageDealt << " targets!\n";
    } else {
        std::cout << "No targets in the area.\n";
    }

    enhancements.reset();

    return true;
}

std::string AreaDamageSpell::getName() const {
    return name_;
}

std::string AreaDamageSpell::getDescription() const {
    return name_ + " (Damage: " + std::to_string(baseDamage_) + 
           ", Range: " + std::to_string(baseRange_) + 
           ", Area: " + std::to_string(baseAreaSize_) + "x" + std::to_string(baseAreaSize_) + ")";
}

SpellType AreaDamageSpell::getType() const {
    return SpellType::AREA_DAMAGE;
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(baseDamage_, baseRange_, baseAreaSize_, name_);
}

bool AreaDamageSpell::isInRange(const Pos& from, const Pos& to, int range) const {
    int distance = std::abs(from.x - to.x) + std::abs(from.y - to.y);
    return distance <= range;
}

void AreaDamageSpell::applyPermanentBonus(const EnhancementState& bonus) {
    baseDamage_ += bonus.damageBonus;
    if (baseDamage_ < 0) baseDamage_ = 0;
    baseRange_ += bonus.rangeBonus;
    if (baseRange_ < 1) baseRange_ = 1;
    baseAreaSize_ += bonus.areaBonus;
    if (baseAreaSize_ < 1) baseAreaSize_ = 1;
}

