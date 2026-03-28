#include "Tower.h"
#include "Player.h"
#include "Chaosbolt.h"

Tower::Tower() : 
    Creature(static_cast<char>(ObjectType::Tower), TOWER_HP, 0), 
    gen(std::random_device{}())
{}

void Tower::attack(Player *player){
    if(this->is_charged){
        Chaosbolt::cast(player, this->gen);
        this->tower_recharge = 2;
        this->is_charged = false;
    }
    else{
        this->tower_recharge--;
        if(this->tower_recharge == 0){
            this->is_charged = true;
        }
    }
}

void Tower::save(std::ofstream& ofs) const {
    Creature::save(ofs);
    ofs << tower_recharge << " ";
    ofs << is_charged << " ";
}

void Tower::load(std::ifstream& ifs) {
    Creature::load(ifs);
    ifs >> tower_recharge;
    ifs >> is_charged;
}