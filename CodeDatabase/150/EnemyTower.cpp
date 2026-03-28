#include "EnemyTower.hpp"
#include "SpellCaster.hpp"
#include "GameState.hpp"
#include "DirectDamageSpell.hpp"
#include <cmath>

EnemyTower::EnemyTower(int x, int y, int health, int attackRange, int attackCooldown)
    : Building(x, y, health), attackRange_(attackRange), 
      attackCooldown_(attackCooldown), currentAttackCooldown_(attackCooldown)
{
    if (attackRange_ <= 0) {
        throw std::invalid_argument("Attack range must be positive");
    }
    if (attackCooldown_ <= 0) {
        throw std::invalid_argument("Attack cooldown must be positive");
    }
    
    attackSpell_ = createAttackSpell();
}

std::unique_ptr<ISpell> EnemyTower::createAttackSpell() {
    return std::make_unique<DirectDamageSpell>(10, attackRange_, 0);
}

void EnemyTower::update() {
    if (isDestroyed()) {
        return;
    }
    
    if (currentAttackCooldown_ < attackCooldown_){
        currentAttackCooldown_++;
    }
}

bool EnemyTower::isActive() const {
    return !isDestroyed();
}

bool EnemyTower::canAttack() const {
    return currentAttackCooldown_ >= attackCooldown_ && !isDestroyed();
}

bool EnemyTower::tryAttack(int targetX, int targetY, GameState& gameState) {
    if (!canAttack()) {
        return false;
    }

    int distance = std::abs(getPositionX() - targetX) + std::abs(getPositionY() - targetY);
    if (distance > attackRange_) {
        return false;
    }
    
    struct TowerSpellCaster : public SpellCaster {
        int x, y;
        TowerSpellCaster(int x, int y) : x(x), y(y) {}
        int getPositionX() const override { return x; }
        int getPositionY() const override { return y; }
    } caster(getPositionX(), getPositionY());
    
    if (attackSpell_->cast(targetX, targetY, gameState, caster)) {
        currentAttackCooldown_ = 0;
        return true;
    }
    
    return false;
}

int EnemyTower::getAttackRange() const {
    return attackRange_;
}