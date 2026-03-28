#ifndef FIGHT_STRATEGY_H_
#define FIGHT_STRATEGY_H_

#include <string>

class FightStrategy {
public:
    virtual ~FightStrategy() = default;
    virtual bool CanAttack(int distance) const = 0; 
    virtual int GetDamage() const = 0;              
    virtual std::string GetModeName() const = 0;  
};

#endif // FIGHT_STRATEGY_H_
