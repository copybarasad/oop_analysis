#include "EnemyTower.hpp"


EnemyTower::EnemyTower() : EnemyTower(1, 5, 5, 3) {}

EnemyTower::EnemyTower(int towerLevel) : EnemyTower(towerLevel, 5, 5, 3) {}

EnemyTower::EnemyTower(int towerLevel, int spellDamage, int spellDistance, int attackPeriod) {
    towerHealth.setMaxHealth(GlobalGameConfig::barracksHealth * towerLevel * GlobalGameConfig::difficulty);
    towerHealth.restoreHealth();
    towerSpell.setBaseDamage(spellDamage);
    towerSpell.setBaseDistance(spellDistance);
    this->attackPeriod = attackPeriod;
    this->attackCooldown = 0;
    this->towerlevel = towerLevel;
}



void EnemyTower::causeDamage(int damage) {
    towerHealth.reduseCurrentHealth(damage);
}


std::pair<int, int>  EnemyTower::getHealth() {
    return {towerHealth.getCurrentHealth(), towerHealth.getMaxHealth()};
}


bool EnemyTower::alive() const {
    return towerHealth.getCurrentHealth();
}


void EnemyTower::towerAttack(GameContext &ctx, int userIndex, int power) {
    if (attackCooldown == 0) {
        towerSpell.incCountOfItem();
        towerSpell.cast(ctx, userIndex, power);
        attackCooldown = attackPeriod;
    }
    else {
        attackCooldown--;
    }
}

int EnemyTower::getSpellDistance() {
    return towerSpell.getBaseDistance();
}

int EnemyTower::getDamage() {
    return 0;
}


int EnemyTower::getLevel() {
    return towerlevel;
}

int EnemyTower::getInt() {
    return towerlevel * 10;
}

char EnemyTower::returnEntitySymbol() {
    return 'T';
}

TowerSaveData EnemyTower::getTowerSaveData() {
    TowerSaveData data;
    data.towerHealth = this->towerHealth.getHealthSavedata();
    data.towerSpell = this->towerSpell.getSpellSaveData();
    data.attackCooldown = this->attackCooldown;
    data.attackPeriod = this->attackPeriod;
    data.towerlevel = this->towerlevel;
    return data;
}


void EnemyTower::setTowerSaveData(TowerSaveData data) {
    this->attackCooldown = data.attackCooldown;
    this->attackPeriod = data.attackPeriod;
    this->towerlevel = data.towerlevel;
    this->towerHealth.setHealthSavedata(data.towerHealth);
    this->towerSpell.setBaseDamage(data.towerSpell.baseDamage);
    this->towerSpell.setBaseDistance(data.towerSpell.baseDistance);
 }