#include "area_damage_spell.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int dmg, int cost, int r) : damage(dmg), cost(cost), radius(r) {}

std::string AreaDamageSpell::getName() const { return "Area_Damage"; }
int AreaDamageSpell::getCost() const { return cost; }
int AreaDamageSpell::getRadius() const { return radius; }
int AreaDamageSpell::getDamage() const { return damage; }

void AreaDamageSpell::upgrade() {
    damage += 15;  
    cost += 5;
}

void AreaDamageSpell::cast(GeneralManager& genMan, std::pair<int, int> target) {
    EnemyManager& enemyMan = genMan.getEnemyManager();
    TowerManager& towerMan = genMan.getTowerManager();
    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            int x = target.first + dx;
            int y = target.second + dy;
            std::cout << "x,y = " << x << y << '\n';
            if (enemyMan.hasEnemyAt(x, y)) {
                genMan.damageEnemyAt(x, y, damage);
            }
            if (towerMan.hasTowerAt(target.first, target.second)) {
                towerMan.damageTowerAt(target.first, target.second, damage);
            }
        }
    }
}