#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int maxHealth, int spawnPeriod, int enemyHealth, int enemyDamage): Character(maxHealth, CharacterCondition::Normal),
spawnPeriod(spawnPeriod), spawnTurns(spawnPeriod), enemyHP(enemyHealth), enemyDMG(enemyDamage) {}

EnemyBuilding::EnemyBuilding():
    EnemyBuilding(DEFAULT_MAX_HEALTH, DEFAULT_MAX_SPAWN_TURNS, DEFAULT_MAX_HEALTH, DEFAULT_DAMAGE) {}

int EnemyBuilding::getTurns() const{
    return this->spawnTurns;
}

void EnemyBuilding::setTurns(int spawnTurns){
    this->spawnTurns = std::max(0, spawnTurns);
}

void EnemyBuilding::setSpawnPeriod(int spawnPeriod){
    this->spawnPeriod = std::max(1, spawnPeriod);
}

int EnemyBuilding::getSpawnPeriod() const{
    return this->spawnPeriod;
}

void EnemyBuilding::setEnemyHP(int enemyHP){
    this->enemyHP = enemyHP;
}

int EnemyBuilding::getEnemyHP(){
    return this->enemyHP;
}

void EnemyBuilding::setEnemyDMG(int enemyDMG){
    this->enemyDMG = enemyDMG;
}

int EnemyBuilding::getEnemyDMG(){
    return this->enemyDMG;
}

void EnemyBuilding::updateCounter(){
    if(this->spawnTurns > 0){
        this->spawnTurns -= 1;
    }
    else{
        this->spawnTurns = this->spawnPeriod;
    }
}

std::string EnemyBuilding::getName(){
    return "Enemy Building";
}