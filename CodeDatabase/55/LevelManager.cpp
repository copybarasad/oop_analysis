#include "LevelManager.h"


LevelManager::LevelManager() : current_level_id(1) {
    loadAllLevels();
}

int LevelManager::getCurrentId() const{
    return current_level_id;
}

bool LevelManager::nextLevel(){
    return next_level_id;
}

const LevelConfig& LevelManager::getCurrentLevel(){
    return levels[current_level_id];
}

LevelConfig& LevelManager::getLevel(int id){
    if(id <= 3 && id >= 1){
        return levels[id];
    }
    return levels[1];
}

void LevelManager::loadAllLevels() {
    LevelConfig l1("НА ТИХОМ БЕРЕГУ", 1, 2);
    l1.sea_width = 10;
    l1.sea_height = 10;
    l1.pl_start_pos = {0, 0};
    l1.en_count = 5;
    l1.en_damage = 1;
    l1.oct_count = 1;
    l1.oct_damage = 2;
    l1.vortex_count = 5;
    l1.health = 10;
    levels[1] = l1;

    LevelConfig l2("КРУШЕНИЕ В ПОДВОДНОЙ ПЕЩЕРЕ", 2, 3);
    l2.sea_width = 20;
    l2.sea_height = 3;
    l2.pl_start_pos = {0, 1};
    l2.en_count = 7;
    l2.en_damage = 2;
    l2.oct_count = 4;
    l2.oct_damage = 3;
    l2.vortex_count = 5;
    l2.health = 10;
    levels[2] = l2;

    LevelConfig l3("ЛОГОВО КРАКЕНА", 3, 0);
    l3.sea_width = 15;
    l3.sea_height = 15;
    l3.pl_start_pos = {7, 7};
    l3.en_count = 10;
    l3.en_damage = 3;
    l3.oct_count = 8;
    l3.oct_damage = 4;
    l3.vortex_count = 10;
    l3.health = 10;
    levels[3] = l3;
}