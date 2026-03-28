#include "DirectDamageSpell.hpp"
#include "GameState.hpp"
#include "SpellCaster.hpp"
#include "EntityManager.hpp"
#include "Hero.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int damage, int range, int manaCost)
    : damage_(damage), range_(range), manaCost_(manaCost), type_(SpellType::DirectDamage) {}

bool DirectDamageSpell::cast(int targetX, int targetY, GameState& gameState, SpellCaster& caster) {
    int casterX = caster.getPositionX();
    int casterY = caster.getPositionY();
    
    if (!checkRange(casterX, casterY, targetX, targetY) || !gameState.field.isValidPosition(targetX, targetY)) {
        return false;
    }
    
    if (casterX == targetX && casterY == targetY) {
        return false;
    }

    const auto& cell = gameState.field.getCell(targetX, targetY);

    bool spellUsed = EntityManager::damageEnemyAt(gameState, targetX, targetY, damage_);
    
    if (!spellUsed) {
        spellUsed = EntityManager::damageAllyAt(gameState, targetX, targetY, damage_);
    }
    
    if (!spellUsed) {
        spellUsed = EntityManager::damageBuildingAt(gameState, targetX, targetY, damage_);
    }
    
    if (!spellUsed && cell.hasPlayer()) {
        gameState.player.takeDamage(damage_);
        spellUsed = true;
    }
    return spellUsed;
}

std::unique_ptr<ISpell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}

bool DirectDamageSpell::checkRange(int casterX, int casterY, int targetX, int targetY) const {
    int distance = std::abs(casterX - targetX) + std::abs(casterY - targetY);
    return distance <= range_;
}

void DirectDamageSpell::enhanceRange(int bonus) {
    range_ += bonus;
}

void DirectDamageSpell::applyEnhancements(int enhancementCount) {
    enhanceRange(enhancementCount);
}

void DirectDamageSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&damage_), sizeof(damage_));
    os.write(reinterpret_cast<const char*>(&range_), sizeof(range_));
    os.write(reinterpret_cast<const char*>(&manaCost_), sizeof(manaCost_));
}

void DirectDamageSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&damage_), sizeof(damage_));
    is.read(reinterpret_cast<char*>(&range_), sizeof(range_));
    is.read(reinterpret_cast<char*>(&manaCost_), sizeof(manaCost_));
}