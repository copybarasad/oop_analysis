#include "ActorEquipment.hpp"


Equipment::Equipment() {
    this->currentWeapon = Equipment::equippedWeapon::MELEE;
}


Equipment::~Equipment() {
}


int Equipment::getAttackBonus() {
    if (currentWeapon == Equipment::equippedWeapon::RANGE) {
        return getRangeAttackBonus();
    }
    else {
        return getMeleeAttackBonus();
    }
}

int Equipment::getMeleeAttackBonus() {
    return meleeWeaponMulti;
}


int Equipment::getRangeAttackBonus() {
    return rangeWeaponMulti;
}


void Equipment::setMeleeWeapon(int newMulti) {
    meleeWeaponMulti = newMulti;
}


void Equipment::setRangeWeapon(int newMulti) {
    rangeWeaponMulti = newMulti;
}


bool Equipment::melle() const{
    return (currentWeapon == Equipment::equippedWeapon::MELEE);
}


bool Equipment::range() const {
    return (currentWeapon == Equipment::equippedWeapon::RANGE);
}


void Equipment::swapWeapon() {
    if (currentWeapon == Equipment::equippedWeapon::RANGE) {
        currentWeapon = Equipment::equippedWeapon::MELEE;
    }
    else {
        currentWeapon = Equipment::equippedWeapon::RANGE;
    }
}


EquipmentSaveData Equipment::getEquipmentSavedata() {
    EquipmentSaveData data;
    data.currentWeapon = (int)this->currentWeapon;
    data.meleeWeaponMulti = this->meleeWeaponMulti;
    data.rangeWeaponMulti = this->rangeWeaponMulti;
    return data;
}


void Equipment::setEquipmentSavedata(EquipmentSaveData data) {
    if (data.currentWeapon) {
        this->currentWeapon = Equipment::equippedWeapon::RANGE;
    }
    else {
        this->currentWeapon = Equipment::equippedWeapon::MELEE;
    }
    this->meleeWeaponMulti = data.meleeWeaponMulti;
    this->rangeWeaponMulti = data.rangeWeaponMulti;
}