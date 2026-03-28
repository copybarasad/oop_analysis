#include "direct_damage_spell.h"

DirectDamageSpell::DirectDamageSpell(int dmg, int cost, int r) : damage(dmg), cost(cost), radius(r) {}

std::string DirectDamageSpell::getName() const { return "Direct_Damage"; }
int DirectDamageSpell::getCost() const { return cost; }
int DirectDamageSpell::getRadius() const { return radius; }
int DirectDamageSpell::getDamage() const { return damage; }

void DirectDamageSpell::upgrade() {
    damage += 15;  
    cost += 5;
}

void DirectDamageSpell::cast(GeneralManager& genMan, std::pair<int, int> target) {
    EnemyManager& enemyMan = genMan.getEnemyManager();
    PlayerManager& playerMan = genMan.getPlayerManager();
    TowerManager& towerMan = genMan.getTowerManager();
    if (enemyMan.hasEnemyAt(target.first, target.second)) {
        genMan.damageEnemyAt(target.first, target.second, damage);
        return;
    }
    if (towerMan.hasTowerAt(target.first, target.second)) {
        towerMan.damageTowerAt(target.first, target.second, damage);
        return;
    }
    playerMan.takePlayerDamage(damage);
}
