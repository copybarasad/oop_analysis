#ifndef GAME_CELL_H
#define GAME_CELL_H

#include "GameEnums.h"
#include "Enemy.h"
#include <memory>
#include <optional>

class GameCell {
private:
    CellType type;
    std::unique_ptr<Enemy> enemy;

public:
    GameCell(CellType cellType = CellType::EMPTY);
    GameCell(const GameCell& other);
    GameCell& operator=(const GameCell& other);
    
    CellType getType() const;
    void setType(CellType cellType);
    
    bool hasEnemy() const;
    std::optional<Enemy> getEnemy() const;
    void setEnemy(std::unique_ptr<Enemy> newEnemy);
    std::unique_ptr<Enemy> removeEnemy();
    
    bool isPassable() const;
};

#endif
