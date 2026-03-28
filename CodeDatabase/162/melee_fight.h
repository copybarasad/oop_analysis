#ifndef MELEE_FIGHT_H_
#define MELEE_FIGHT_H_

#include "fight_strategy.h"

class MeleeFight : public FightStrategy {
public:
    bool CanAttack(int distance) const override {
        return distance == 1; 
    }
    int GetDamage() const override {
        return 30;  
    }
    std::string GetModeName() const override {
        return "melee";
    }
};

#endif // MELEE_FIGHT_H_
