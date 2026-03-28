#include "DirectDamageSpell.h"
#include "IGameWorldFacade.h"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int damage, int range, const std::string& name)
    : baseDamage_(damage), baseRange_(range), name_(name) {}

bool DirectDamageSpell::cast(SpellContext& context, EnhancementState& enhancements) {
    if (!context.world) {
        return false;
    }
    
    Pos playerPos = context.world->getPlayerPosition();
    Pos target = context.targetPosition;

    int effectiveRange = baseRange_ + enhancements.rangeBonus;
    int effectiveDamage = baseDamage_ + enhancements.damageBonus;

    if (!context.world->isInRange(playerPos, target, effectiveRange)) {
        std::cout << "Target out of range! Max range: " << effectiveRange << "\n";
        return false;
    }

    if (context.world->dealDamageToEnemy(target, effectiveDamage)) {
        std::cout << "Cast " << name_ << "! Dealt " << effectiveDamage << " damage to enemy!\n";
        enhancements.reset();
        return true;
    }

    if (context.world->dealDamageToBuilding(target, effectiveDamage)) {
        std::cout << "Cast " << name_ << "! Dealt " << effectiveDamage << " damage to building!\n";
        enhancements.reset();
        return true;
    }

    if (context.world->dealDamageToTower(target, effectiveDamage)) {
        std::cout << "Cast " << name_ << "! Dealt " << effectiveDamage << " damage to tower!\n";
        enhancements.reset();
        return true;
    }
    
    std::cout << "No valid target at that position!\n";
    return false;
}

std::string DirectDamageSpell::getName() const {
    return name_;
}

std::string DirectDamageSpell::getDescription() const {
    return name_ + " (Damage: " + std::to_string(baseDamage_) + ", Range: " + std::to_string(baseRange_) + ")";
}

SpellType DirectDamageSpell::getType() const {
    return SpellType::DIRECT_DAMAGE;
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(baseDamage_, baseRange_, name_);
}

bool DirectDamageSpell::isInRange(const Pos& from, const Pos& to, int range) const {
    int distance = std::abs(from.x - to.x) + std::abs(from.y - to.y);
    return distance <= range;
}

void DirectDamageSpell::applyPermanentBonus(const EnhancementState& bonus) {
    baseDamage_ += bonus.damageBonus;
    if (baseDamage_ < 0) baseDamage_ = 0;
    baseRange_ += bonus.rangeBonus;
    if (baseRange_ < 1) baseRange_ = 1;
}

