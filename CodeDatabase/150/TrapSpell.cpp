#include "TrapSpell.hpp"
#include "GameState.hpp"
#include "SpellCaster.hpp"
#include <memory>
#include <cmath>
#include <iostream>

TrapSpell::TrapSpell(int damage, int range, int manaCost)
    : damage_(damage), range_(range), manaCost_(manaCost), type_(SpellType::Trap) {}

bool TrapSpell::cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) {
    int casterX = caster.getPositionX();
    int casterY = caster.getPositionY();
    
    if (!checkRange(casterX, casterY, targetX, targetY) || !gameState.field.isValidPosition(targetX, targetY)) {
        return false;
    }
    
    const auto& cell = gameState.field.getCell(targetX, targetY);
    
    if (!cell.isEmpty()) {
        return false;
    }
    
    Trap trap(targetX, targetY, damage_);
    gameState.traps.push_back(trap);
    return true;
}

std::unique_ptr<ISpell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(*this);
}

bool TrapSpell::checkRange(int casterX, int casterY, int targetX, int targetY) const {
    int distance = std::abs(casterX - targetX) + std::abs(casterY - targetY);
    return distance <= range_;
}

void TrapSpell::enhanceDamage(int bonus) {
    damage_ += bonus;
}

void TrapSpell::applyEnhancements(int enhancementCount) {
    enhanceDamage(5 * enhancementCount);
}

void TrapSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&damage_), sizeof(damage_));
    os.write(reinterpret_cast<const char*>(&range_), sizeof(range_));
    os.write(reinterpret_cast<const char*>(&manaCost_), sizeof(manaCost_));
}

void TrapSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&damage_), sizeof(damage_));
    is.read(reinterpret_cast<char*>(&range_), sizeof(range_));
    is.read(reinterpret_cast<char*>(&manaCost_), sizeof(manaCost_));
}