#pragma once
#include "../core/Config.hpp"
#include "../core/SaveData.hpp"

class Attack {
private:
    int attack;
public:
    AttackSaveData getAttackSavedata();
    void setAttackSaveData(AttackSaveData data);
    void setAttack(int attack);
    int getAttack() const;
    Attack();
    Attack(int attack);
    ~Attack();
};
