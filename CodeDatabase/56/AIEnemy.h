#ifndef AIENEMY_H
#define AIENEMY_H

#include "player.h"
#include "enemy.h"

class AIEnemy{
public:
    static bool choice_attack_go(Enemy& enemy, Player& gamer);
    static std::pair<int, int> alg_finding_way(Enemy& enemy, Player& gamer);
};
#endif