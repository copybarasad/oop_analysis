#include "trap_spell.h"
#include "trap_context.h"
#include "player.h"
#include "game_field.h"
#include <iostream>
#include <cmath>
#include <sstream>

TrapSpell::TrapSpell(int damage, int radius) 
    : damage(damage), radius(radius) {}

bool TrapSpell::apply(const TrapContext& context) {
    if (!canUse(context)) {
        return false;
    }
    
    context.field.placeTrap(context.targetX, context.targetY, damage);
    std::cout << "Trap spell! Placed trap at (" << context.targetX << "," << context.targetY 
              << ") dealing " << damage << " damage.\n";
    
    return true;
}

bool TrapSpell::canUse(const TrapContext& context) const {
    auto playerPos = context.player.getPosition();
    int distance = std::abs(context.targetX - playerPos.first) + 
                   std::abs(context.targetY - playerPos.second);
    
    return distance <= radius && distance > 0 && context.field.isValidPosition(context.targetX, context.targetY) 
           && context.field.isPassable(context.targetX, context.targetY);
}

const char* TrapSpell::getName() const {
    return "Trap";
}

std::unique_ptr<Spell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(damage, radius);
}

int TrapSpell::getDamage() const {
    return damage;
}

void TrapSpell::setDamage(int newDamage) {
    damage = newDamage;
}

void TrapSpell::applyEnhancement() {
    setDamage(getDamage() + 5);
    std::cout << "  [Enhanced] Trap damage increased to " << getDamage() << "\n";
}

std::string TrapSpell::getSerializedData() const {
    std::ostringstream oss;
    oss << "Trap " << damage;
    return oss.str();
}