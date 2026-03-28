#ifndef TRAP_MANAGER_H
#define TRAP_MANAGER_H

#include "Trap.h"
#include "Enemy.h"
#include "Player.h"
#include <vector>

class TrapManager {
public:
    static void checkTrapCollisions(std::vector<Trap> &traps, std::vector<Enemy> &enemies, Player &player);
    static void addTrap(std::vector<Trap> &traps, const Trap &trap);
    static void removeTrapAt(std::vector<Trap> &traps, const Position &pos);
};

#endif