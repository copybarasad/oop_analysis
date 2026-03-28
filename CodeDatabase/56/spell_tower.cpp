#include "spell_tower.h"

SpellTower::SpellTower(int X, int Y, int hp, std::string combat, int RANGED, int froz, int r_c) 
    : GameObject(X, Y, hp, combat, RANGED, froz),
      recharge_cast(r_c), timer_cast(r_c),
      spell(std::make_unique<DirectedSpell>(1, "explosion", 5, 15, 3)) {
}
DirectedSpell& SpellTower::getSpell(){ return *spell; }
int SpellTower::getRechargeCast(){ return recharge_cast; }   
int SpellTower::getTimerCast(){ return timer_cast; }
int SpellTower::getDamage() { return 0; } 
void SpellTower::TimerReduce(){
    timer_cast--;
}
nlohmann::json SpellTower::toJson() const {
    return {
        {"x", x},
        {"y", y},
        {"health", HP},
        {"recharge_cast", recharge_cast},
        {"timer_cast", timer_cast},
        {"combat_type", type_combat},
        {"range", ranged},
        {"frozen", frozen},
        {"alive", flag_life},
        {"spell", spell->toJson()}
    };
}
void SpellTower::fromJson(const nlohmann::json& j) {
    x = j["x"];
    y = j["y"];
    HP = j["health"];
    recharge_cast = j["recharge_cast"];
    timer_cast = j["timer_cast"];
    type_combat = j["combat_type"];
    ranged = j["range"];
    frozen = j["frozen"];
    flag_life = j["alive"];
    
    spell = std::make_unique<DirectedSpell>(1, "explosion", 5, 15, 3);
    spell->fromJson(j["spell"]);
}
void SpellTower::setTimer(int time){
    timer_cast = time;
}