#ifndef ENEMY_H
#define ENEMY_H
#include "basic_info.h"

class Player;
class Place;

class Enemy: public BasicInfo{

public:
    Enemy(int maxHealth_ = 20, int health_ = 20, int damage_ = 2, int x_ = 5, int y_ = 5, int points_ = 1);
    
    void action(Player& player, Place& place);
    
    void print_current_stats();
    
    ~Enemy();
};
#endif