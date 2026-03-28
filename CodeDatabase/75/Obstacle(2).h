#pragma once
#include "Cell.h"
#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>

class GameField;

class Obstacle {
public:
    enum class Shape {
        SQUARE,
        CROSS,
        LINE,
        TREE,
        PILLAR
    };
    Obstacle(int centerX, int centerY, Shape shape, int size, sf::Color color);
    
    const std::vector<std::pair<int, int>>& getBlocks() const { return m_blocks; }
    sf::Color getColor() const { return m_color; }
    
    void drawOnField(GameField& field) const;
private:
    std::vector<std::pair<int, int>> m_blocks;
    Shape m_shape;
    int m_size;
    sf::Color m_color;
    
    void generateSquare(int cx, int cy);
    void generateCross(int cx, int cy);
    void generateLine(int cx, int cy);
    void generateTree(int cx, int cy);
    void generatePillar(int cx, int cy); 
};