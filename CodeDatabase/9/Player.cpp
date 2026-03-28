#include "Player.h"
#include "Config.h"

Player::Player(int hp, int meleeDamage,int rangedDamage, std::pair<int, int> pos): Character(PLAYER_HP, PLAYER_MELEE_DAMAGE, pos), hand_(HAND_CAPACITY){
    score_ = 0;
    rangedMode_ = false;
    meleeDamage_ = PLAYER_MELEE_DAMAGE;
    rangedDamage_= PLAYER_RANGED_DAMAGE;
    skipNextTurn_ = false;
    damage_ = meleeDamage_;
}

int Player::score(){
    return score_;
}

void Player::addScore(int value){
    if (value > 0) score_ += value;
}

void Player::step(std::pair<int, int> delta){
    moveBy(delta);
}

void Player::AttackMobile(){
    rangedMode_ = !rangedMode_;
    if (rangedMode_) damage_ = rangedDamage_;
    else damage_ = meleeDamage_;
}

bool Player::isMode(){
    return rangedMode_;
}

bool Player::shouldSkipTurn(){
    return skipNextTurn_;
}

void Player::setSkipNextTurn(bool v){
    skipNextTurn_ = v;
}

Hand& Player::hand(){
    return hand_;
}

bool Player::spendScore(int value){
    if(value <= 0){
        return false;
    }
    if(score_ < value){
        return false;
    }
    score_ -= value;
    return true;
}


void Player::AddPendingModifier(const SpellModifier& m){
    pendingMod_.radiusDelta += m.radiusDelta;
    pendingMod_.aoeSizeDelta += m.aoeSizeDelta;
    pendingMod_.damageDelta += m.damageDelta;
    pendingMod_.summonCountDelta += m.summonCountDelta;
}

bool Player::HasPendingModifier() const{
    return pendingMod_.radiusDelta != 0 || pendingMod_.aoeSizeDelta != 0 || pendingMod_.damageDelta != 0 || pendingMod_.summonCountDelta != 0;
}

SpellModifier Player::PeekPendingModifier() const{
    return pendingMod_;
}

SpellModifier Player::TakePendingModifier(){
    SpellModifier out = pendingMod_;
    pendingMod_ = SpellModifier();
    return out;
}