#pragma once
#include "../core/SaveData.hpp"

class Equipment {
private:
    enum class equippedWeapon { MELEE, RANGE } currentWeapon;
    int meleeWeaponMulti = 3;
    int rangeWeaponMulti = 2;
    int getMeleeAttackBonus();
    int getRangeAttackBonus();
public:
    Equipment();
    ~Equipment();
    EquipmentSaveData getEquipmentSavedata();
    void setEquipmentSavedata(EquipmentSaveData data);
    int getAttackBonus();
    void setMeleeWeapon(int newMulti);
    void setRangeWeapon(int newMulti);
    bool melle() const;
    bool range() const;
    void swapWeapon();
};