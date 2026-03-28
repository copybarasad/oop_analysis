#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include "Position.h"
#include "Trap.h"
#include <vector>
#include <utility>
#include <memory>

class EnemyManager;

class TrapManager {
private:
    std::vector<std::pair<Position, std::unique_ptr<Trap>>> traps;

public:
    void addTrap(const Position& pos, std::unique_ptr<Trap> trap);
    bool hasTrapAt(const Position& pos) const;
    int triggerTrap(const Position& pos, EnemyManager& enemyManager);
    
    const std::vector<std::pair<Position, std::unique_ptr<Trap>>>& getTraps() const;
};

#endif
