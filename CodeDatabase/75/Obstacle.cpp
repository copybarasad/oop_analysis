#include "Obstacle.h"
#include "GameField.h" 
#include <cstdlib>

Obstacle::Obstacle(int centerX, int centerY, Shape shape, int size, sf::Color color)
    : m_shape(shape), m_size(size), m_color(color) {
    
    switch (m_shape) {
        case Shape::SQUARE: generateSquare(centerX, centerY); break;
        case Shape::CROSS:  generateCross(centerX, centerY); break;
        case Shape::LINE:   generateLine(centerX, centerY); break;
        case Shape::TREE:   generateTree(centerX, centerY); break;
        case Shape::PILLAR: generatePillar(centerX, centerY); break;
    }
}

void Obstacle::drawOnField(GameField& field) const {
    for (const auto& block : m_blocks) {
        int x = block.first;
        int y = block.second;
        if (field.isInBounds(x, y)) {
            // We encode Obstacle logic in Cell type, but Color is stored in Obstacle object
            // Render will draw Obstacle object over the field
            field.getCell(x, y).setType(Cell::OBSTACLE);
        }
    }
}

void Obstacle::generateSquare(int cx, int cy) {
    for (int x = cx - m_size; x <= cx + m_size; ++x)
        for (int y = cy - m_size; y <= cy + m_size; ++y)
            m_blocks.emplace_back(x, y);
}

void Obstacle::generateCross(int cx, int cy) {
    for (int x = cx - m_size; x <= cx + m_size; ++x) m_blocks.emplace_back(x, cy);
    for (int y = cy - m_size; y <= cy + m_size; ++y) m_blocks.emplace_back(cx, y);
}

void Obstacle::generateLine(int cx, int cy) {
    bool horiz = (rand() % 2 == 0);
    if (horiz) for (int x = cx - m_size; x <= cx + m_size; ++x) m_blocks.emplace_back(x, cy);
    else for (int y = cy - m_size; y <= cy + m_size; ++y) m_blocks.emplace_back(cx, y);
}

void Obstacle::generateTree(int cx, int cy) {
    // Random blob
    m_blocks.emplace_back(cx, cy);
    for(int i=0; i<m_size*3; ++i) {
        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;
        m_blocks.emplace_back(cx+dx, cy+dy);
        if(rand()%2==0) m_blocks.emplace_back(cx+dx*2, cy+dy*2);
    }
}

void Obstacle::generatePillar(int cx, int cy) {
    // Solid block
    for(int x=cx; x<cx+2; ++x)
        for(int y=cy; y<cy+2; ++y)
            m_blocks.emplace_back(x, y);
}