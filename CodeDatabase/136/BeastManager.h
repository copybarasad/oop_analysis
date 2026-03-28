#ifndef BEAST_MANAGER_H
#define BEAST_MANAGER_H

#include "Beast.h"
#include "GameField.h"
#include "Wanderer.h"
#include <vector>

class BeastManager {
private:
    std::vector<std::pair<Position, Beast>> beasts;
    GameField& field;
    Position wandererPosition;
    Wanderer& wanderer;

public:
    BeastManager(GameField& field, const Position& wandererPos, Wanderer& wanderer);
    
    void moveBeasts();
    void addBeast(const Position& pos, const Beast& beast);
    void removeDeadBeasts();
    Beast* getBeastAt(const Position& pos);
    const std::vector<std::pair<Position, Beast>>& getBeasts() const;
    void clearBeasts();
    int countDeadBeasts() const;
    void updateWandererPosition(const Position& newPos) {
        wandererPosition = newPos;
    }
    ~BeastManager();
};

#endif