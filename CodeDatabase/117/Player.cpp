#include "Player.h"
#include "../LogSystem/Notifier.h"
#include "../LogSystem/DamageEvent.h"
#include "../LogSystem/PlayerGainEXPEvent.h"

Player::Player(int maxHealth, CharacterCondition condition, int meleeDamage, int rangeDamage, int startExpoints):
    Character(maxHealth, condition), meleeDamage(meleeDamage), rangeDamage(rangeDamage),
    expoints(startExpoints), damageMode(DamageMode::Melee) {}

Player::Player(): Player(DEFAULT_MAX_HEALTH, CharacterCondition::Normal, DEFAULT_MELEE_DAMAGE, DEFAULT_RANGE_DAMAGE, DEFAULT_START_EXP) {}

void Player::setEXP(int expoints){
    this->expoints = expoints;
}

int Player::getEXP() const{
    return this->expoints;
}

void Player::addEXP(int addingEXP){
    this->expoints += addingEXP;
}

void Player::subEXP(int subEXP){
    this->expoints = std::max(this->expoints - subEXP, 0);
}

void Player::setDMG(int damage){
    if (this->damageMode == DamageMode::Melee){
        this->meleeDamage = damage;
    }
    else{
        this->rangeDamage = damage;
    }
}

int Player::getDMG() const{
    if (this->damageMode == DamageMode::Melee){
        return this->meleeDamage;
    }
    else{
        return this->rangeDamage;
    }
}

DamageMode Player::getDMGMode(){
    return this->damageMode;
}

void Player::switchDMGMode(){
    if (this->damageMode == DamageMode::Melee){
        this->damageMode = DamageMode::Range;
    }
    else{
        this->damageMode = DamageMode::Melee;
    }
}

bool Player::attack(Character& target){
    if ((this->getDMGMode() == DamageMode::Melee && (this->squareDistTo(target) <= DISTANCE_FOR_MELEE + DISTANCE_FOR_MELEE))
    || (this->getDMGMode() == DamageMode::Range && this->squareDistTo(target) <= DISTANCE_FOR_RANGE * DISTANCE_FOR_RANGE)){
        target.takeDMG(this->getDMG());
        DamageEvent DMGevent = DamageEvent("Player", target.getName(), this->getDMG());
        Notifier::getInstance().notify(DMGevent);

        if (!target.isAlive()){
            this->addEXP(EXP_FOR_KILL);
            PlayerGainEXPEvent event = PlayerGainEXPEvent(EXP_FOR_KILL);
            Notifier::getInstance().notify(event);
        }
        return true;
    }
    return false;
}

std::string Player::getName(){
    return "Player";
}