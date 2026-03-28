#include "tower.h"
#include "field.h"
#include "combat_system.h"
#include "direct_damage_spell.h"

Tower::Tower(int id, int health, int attackDamage, int attackRange, int cooldown)
: Building(id, health, 0), attackDamage_(attackDamage), attackRange_(attackRange),
  cooldown_(cooldown), cooldownCounter_(0) {
    towerSpell_ = new DirectDamageSpell(attackDamage_ / 2, attackRange_);
}

Tower::~Tower() {
    if (towerSpell_) delete towerSpell_;
}

EntityType Tower::getType() const { return EntityType::Building; }

void Tower::tick() {
    if (cooldownCounter_ > 0) --cooldownCounter_;
}

void Tower::tryAttack(Field& field) {
    if (cooldownCounter_ > 0) return;
    int row, col;
    if (!field.getPosition(id_, row, col)) return;

    int playerId = field.findPlayerInRadius(row, col, attackRange_);
    if (playerId < 0) return;

    int prow, pcol;
    if (!field.getPosition(playerId, prow, pcol)) return;

    if (towerSpell_->apply(id_, field, prow, pcol)) {
        cooldownCounter_ = cooldown_;
    }
}

void Tower::tryAttackIfTower(Field& field) {
    tryAttack(field);
}

char Tower::getDisplaySymbol() const { return 'T'; }

Tower* Tower::asTower() { return this; }
