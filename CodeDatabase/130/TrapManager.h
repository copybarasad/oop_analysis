#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include "Trap.h"
#include <vector>
#include <memory>

class Enemy;

class TrapManager {
private:
    std::vector<std::unique_ptr<Trap>> traps_;

public:
    TrapManager() = default;

    void addTrap(std::unique_ptr<Trap> trap);

    bool hasTrapOnCell(int x, int y) const;

    int activateTrap(int x, int y);

    int getTrapCount() const;
    
    const std::vector<std::unique_ptr<Trap>>& getTraps() const { return traps_; }
};

#endif

