#ifndef SPELL_TOWER_H
#define SPELL_TOWER_H

#include "game_objects.h"
#include "directed_spell.h"
#include <memory>
#include <nlohmann/json.hpp>

class SpellTower : public GameObject{
private:
    int recharge_cast;
    int timer_cast;
    std::unique_ptr<DirectedSpell> spell;
public:
    SpellTower(int X, int Y, int hp, std::string combat, int RANGED, int froz, int r_c);
    DirectedSpell& getSpell();
    
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);

    int getRechargeCast();
    int getTimerCast();
    int getDamage();
    void TimerReduce();
    void setTimer(int time);
};

#endif