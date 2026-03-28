#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Cell.h"
#include <vector>
#include <memory>
#include <string>

class GameField {
private:
    std::vector<std::vector<Cell>> field;
    int width;
    int height;
    
    void initializeField();
    void placeObstacles();
    void placeSlowingCells();

public:
    GameField(int fieldWidth, int fieldHeight);
    
    // Copy constructor
    GameField(const GameField& other);
    
    // Move constructor
    GameField(GameField&& other) noexcept;
    
    // Copy assignment operator
    GameField& operator=(const GameField& other);
    
    // Move assignment operator
    GameField& operator=(GameField&& other) noexcept;
    
    // Destructor
    ~GameField() = default;
    
    // Getters
    int getWidth() const;
    int getHeight() const;
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    
    // Field operations
    bool canMoveTo(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    void setPlayerPosition(int x, int y);
    void setEnemyPosition(int x, int y);
    void setBuildingPosition(int x, int y);
    void setAllyPosition(int x, int y);
    void setTrap(int x, int y, int damage);
    void clearPosition(int x, int y);
    
    // Utility methods
    void displayField() const;
    
    // Serialization
    std::string serialize() const;
    bool deserialize(const std::string& data);
};

#endif // GAMEFIELD_H

