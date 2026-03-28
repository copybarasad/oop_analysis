#include "TrapSpell.h"
#include "IGameWorldFacade.h"
#include <iostream>
#include <cmath>

TrapSpell::TrapSpell(int damage, int range, const std::string& name, TrapManager* tm)
    : baseDamage_(damage), baseRange_(range), name_(name), trapManager_(tm) {}

bool TrapSpell::cast(SpellContext& context, EnhancementState& enhancements) {
    if (!context.world) {
        return false;
    }
    
    Pos playerPos = context.world->getPlayerPosition();
    Pos target = context.targetPosition;

    if (!context.world->isInRange(playerPos, target, baseRange_)) {
        std::cout << "Target out of range! Max range: " << baseRange_ << "\n";
        return false;
    }

    if (!context.world->isValidPosition(target)) {
        std::cout << "Cannot place trap on invalid position!\n";
        return false;
    }

    int effectiveDamage = baseDamage_ + enhancements.damageBonus;

    if (context.world->placeTrap(target, effectiveDamage)) {
        std::cout << "Placed " << name_ << " (Damage: " << effectiveDamage << ") at (" 
                  << target.x << "," << target.y << ")\n";
        enhancements.reset();
        return true;
    }
    
    std::cout << "Cannot place trap at that location!\n";
    return false;
}

std::string TrapSpell::getName() const {
    return name_;
}

std::string TrapSpell::getDescription() const {
    return name_ + " (Damage: " + std::to_string(baseDamage_) + 
           ", Range: " + std::to_string(baseRange_) + ")";
}

SpellType TrapSpell::getType() const {
    return SpellType::TRAP;
}

std::unique_ptr<Spell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(baseDamage_, baseRange_, name_, trapManager_);
}

bool TrapSpell::isInRange(const Pos& from, const Pos& to, int range) const {
    int distance = std::abs(from.x - to.x) + std::abs(from.y - to.y);
    return distance <= range;
}

void TrapSpell::applyPermanentBonus(const EnhancementState& bonus) {
    baseDamage_ += bonus.damageBonus;
    if (baseDamage_ < 0) baseDamage_ = 0;
    baseRange_ += bonus.rangeBonus;
    if (baseRange_ < 1) baseRange_ = 1;
}

