#ifndef CELL_H
#define CELL_H

#include <string>

enum class CellType {
    EMPTY,
    OBSTACLE,
    SLOWING
};

class Cell {
private:
    CellType type;
    bool hasPlayer;
    bool hasEnemy;
    bool hasBuilding;
    bool hasAlly;
    bool hasTrap;
    int trapDamage;

public:
    Cell();
    Cell(CellType cellType);
    
    // Getters
    CellType getType() const;
    bool hasPlayerOnCell() const;
    bool hasEnemyOnCell() const;
    bool hasBuildingOnCell() const;
    bool hasAllyOnCell() const;
    bool hasTrapOnCell() const;
    bool isEmpty() const;
    bool isPassable() const;
    int getTrapDamage() const;
    
    // Setters
    void setType(CellType cellType);
    void setPlayerPresence(bool presence);
    void setEnemyPresence(bool presence);
    void setBuildingPresence(bool presence);
    void setAllyPresence(bool presence);
    void setTrap(bool hasTrap, int damage = 0);
    
    // Utility methods
    void clearOccupants();
    void clearTrap();
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
};

#endif // CELL_H

