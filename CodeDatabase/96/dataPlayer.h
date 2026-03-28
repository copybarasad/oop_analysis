#pragma once
#include <vector>
//#include "../Player.h"
#include "../AttackMode.h"

struct dataPlayer{
    std::pair<int,int> pos;
    AttackMode mode;
    int score;
    int damage;
    int health;
    int stunDuration;
};