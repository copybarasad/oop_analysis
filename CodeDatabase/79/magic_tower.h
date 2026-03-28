#ifndef MAGIC_TOWER_H
#define MAGIC_TOWER_H

#include "basic_info.h"
#include "weak_default_spell.h"
#include "player.h"
#include "place.h"
#include <vector>

class MagicTower : public BasicInfo {
private:
    int castInterval;
    int castCounter;
    int radius;
    weak_default_spell spell;

public:
    MagicTower(int spawnInterval_, int maxHealth_ = 50, int health_ = 50, int x_ = 9, int y_ = 3, int radius = 3, int points_ = 3);
    
    void update(Place& place, Player& player, std::vector<Enemy> enemies, Tower& tower);
    void print_current_stats();
    ~MagicTower();
};

#endif