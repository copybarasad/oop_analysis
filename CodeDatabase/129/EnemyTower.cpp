#include "EnemyTower.h"

EnemyTower::EnemyTower() {}

EnemyTower::EnemyTower(Point pos){
    coords.x = pos.x;
    coords.y = pos.y;
    regenerate_moves = ENEMY_TOWER::ET_REGENERATE_MOVES;
    dd_spell.set_damage(ENEMY_TOWER::ET_DAMAGE);
}

std::vector<Point> EnemyTower::get_dests(){
    std::vector<Point> dests;
    std::string directions = "wasd";
    for(char c : directions){
        std::vector<Point> tmp = dd_spell.get_dests(coords, c);
        dests.insert(dests.end(), tmp.begin(), tmp.end());
    }

    return dests;
}

void EnemyTower::reset_regenerate_moves(){
    regenerate_moves = 0;
}

void EnemyTower::set_regenerate_moves(){
    regenerate_moves = ENEMY_TOWER::ET_REGENERATE_MOVES;
}

int EnemyTower::get_regenerate_moves(){
    return regenerate_moves;
}

Point EnemyTower::get_coords(){
    return coords;
}

bool EnemyTower::time_to_kill(){
    return regenerate_moves <= 0;
}

int EnemyTower::get_damage(){
    return dd_spell.get_damage();
}

void EnemyTower::wait_moves(){
    regenerate_moves--;
}

void EnemyTower::dont_wait_moves(){
    regenerate_moves++;
}

EnemyTower::~EnemyTower() {}
