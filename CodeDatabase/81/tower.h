#ifndef TOWER_H
#define TOWER_H

#include <random>
#include "Creature.h" 

class Player;
class Game;
class Chaosbolt;

class Tower: public Creature{
    int tower_recharge = 0;
    int max_tower_recarge = 2;
    int tower_range = 3;
    bool is_charged = true;
    std::mt19937 gen;
public:
    Tower();
    void attack(Player *player);
    void save(std::ofstream& ofs) const override;
    void load(std::ifstream& ifs) override;
    friend class Game;
};

#endif