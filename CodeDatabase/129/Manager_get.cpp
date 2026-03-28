#include "Manager.h"

bool Manager::get_hand_can_use_spell(char c){
    return hand.can_use_spell(c);
}

int Manager::get_moves() {
    return moves;
}

int Manager::get_player_lifes(){
    return player.get_lifes();
}

int Manager::get_hand_max_count(){
    return hand.get_max_count();
}

std::vector<std::string> Manager::hand_get_my_spells(){
    return hand.get_my_spells();
}

bool Manager::get_player_is_close_combat(){
    return player.is_close_combat();
}

bool Manager::get_enemy_tower_time_to_kill(){
    return enemy_tower.time_to_kill();
}

std::map<char, int> Manager::get_killed(){
    return killed;
}

int Manager::get_level(){
    return current_level;
}

