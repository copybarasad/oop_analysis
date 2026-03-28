#include <iostream>
#include "player.h"
#include "enemy.h"
#include "AttackType.h"

Player::Player(): speed(2),  maxhealth(100), health(100), meleeDamage(15), rangedDamage(10),  countmurder(0), currentAttackType(AttackType::MELEE) {}

int Player::GetHealth() const {
    return health;
}
int Player::GetMaxHealth() const {
    return maxhealth;
}
int Player::GetSpeed() const {
    return speed;
}
int Player::GetMurder() const{
    return countmurder;
}
int Player::GetDamage() const {
    return (currentAttackType == AttackType::MELEE) ? meleeDamage : rangedDamage;
}
void Player::NewMurder(){
    countmurder++;
}
void Player::Damage(int EnemyDamage){
    health -= EnemyDamage;
    if(health < 0) health = 0;
}
bool Player::IsLife() const{
    return health > 0;
}
void Player::SwitchAttackType() {
    currentAttackType = (currentAttackType == AttackType::MELEE) ? AttackType::RANGED : AttackType::MELEE;
}
int Player::GetMeleeDamage() const{
    return meleeDamage;
}
int Player::GetRangedDamage() const{
    return rangedDamage;
}
AttackType Player::GetAttackType() const{
    return currentAttackType;
}
bool Player::PlayerAttack(Enemy& enemy){ //возвращает тру если враг был побеждне по итогу 
    if (!enemy.IsLife()) return false;
    //удар игрока
    if (currentAttackType == AttackType::MELEE){
        enemy.Damage(GetMeleeDamage());
    }else{
        enemy.Damage(GetRangedDamage());
    }

    if (!enemy.IsLife()){
        NewMurder();
        Update(countmurder);
        return true;
    } else {
        return false;
    }
    
}
void Player::Update(int count){
    if(count%2 == 0 && count > 0){
        health += 5;
        speed += 1;
        meleeDamage += 1;
        rangedDamage += 1;
        if (health > maxhealth){
            health = maxhealth;
        }
    }
}

void Player::RestoreHealth(){
    health = maxhealth;
}

void Player::UpdateMaxHealth(int item) {
    maxhealth += item;
    health = maxhealth; 
}

void Player::UpdateMeleeDamage(int item) {
    meleeDamage += item;
}

void Player::UpdateRangedDamage(int item) {
    rangedDamage += item;
}

void Player::UpdateSpeed(int item) {
    speed += item;
}

