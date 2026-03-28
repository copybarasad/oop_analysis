#pragma once
#include "Cell.h"
#include "Enemy.h"
#include <vector>
#include <string>

class Enemy;

class GameField {
public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    
    void draw(int playerX, int playerY, const std::vector<Enemy*>& enemies) const;
    
    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
    bool isInBounds(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

    void setFloorTexture(const std::string& texName) { m_floorTexture = texName; }
    std::string getFloorTexture() const { return m_floorTexture; }

private:
    std::vector<std::vector<Cell>> m_grid;
    int m_width;
    int m_height;
    std::string m_floorTexture = "floor_tex";
    void initialize();
};