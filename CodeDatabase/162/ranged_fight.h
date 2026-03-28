 #ifndef RANGED_FIGHT_H_
#define RANGED_FIGHT_H_

#include "fight_strategy.h"

class RangedFight : public FightStrategy {
public:
    bool CanAttack(int distance) const override {
        return distance <= 5; 
    }
    int GetDamage() const override {
        return 15; 
    }
    std::string GetModeName() const override {
        return "ranged";
    }
};

#endif // RANGED_FIGHT_H_


