#include "player.hpp"

Player::Player(int startHealth, int startDamage, int startX, int startY) 
    : Character(startHealth, startDamage, startX, startY) {}
void Player::addScore(int points) { 
    score += points; 
}
int Player::getScore() { 
    return score; 
}
WeaponType Player::getWeapon(){
    return weapon;
}
void Player::setWeapon(WeaponType newWeapon){
    weapon = newWeapon;
}
int Player::getRange(){
    return range;
}
void Player::setRange(int newRange){
    range = newRange;
}
void Player::switchWeapons(){
    if(getWeapon() == MELEE) {
        setWeapon(RANGED);
        setRange(3);
    }
    else if(getWeapon() == RANGED){
        setWeapon(MELEE);
        setRange(1);
    } 
}
