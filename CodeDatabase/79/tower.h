#ifndef TOWER_H
#define TOWER_H

#include "basic_info.h"
#include "place.h"
#include "enemy.h"
#include <vector>

class Tower : public BasicInfo {
private:
    int spawnCounter;
    int spawnInterval;
    std::vector<Enemy>& enemies;

public:
    Tower(int spawnInterval_, std::vector<Enemy>& enemies_, int maxHealth_ = 50, int health_ = 50, int x_ = 7, int y_ = 7, int points_ = 3);
    
    void update(Place& place);
    void print_current_stats();
    ~Tower();
};

#endif