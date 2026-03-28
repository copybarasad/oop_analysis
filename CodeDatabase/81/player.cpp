#include "Player.h"

Player::Player() : 
    Creature(static_cast<char>(ObjectType::Player), PLAYER_HP, PLAYER_MELEE_DAMAGE)
{}

int Player::getRange() const { return current_range; } 

void Player::change_fight_style(){
    is_melee = !is_melee;
    if(is_melee){
        this->current_damage = this->melee_damage;
        this->current_range = this->melee_range;
    }
    else{
        this->current_damage = this->range_damage;
        this->current_range = this->far_range;
    }
}

void Player::kill_up(){
    this->kills++;
}

int Player::kills_to_spells(){
    int spells = this->kills / 2;
    this->kills = this->kills % 2;
    return spells;
}

void Player::save(std::ofstream& ofs) const {
    Creature::save(ofs);
    ofs << is_melee << " ";
    ofs << kills << " ";
    ofs << current_range << " ";
}

void Player::load(std::ifstream& ifs) {
    Creature::load(ifs);
    ifs >> is_melee;
    ifs >> kills;
    ifs >> current_range;
}