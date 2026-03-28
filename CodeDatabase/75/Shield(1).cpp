#include "Shield.h"
#include "GameField.h"
#include "Cell.h"
#include <iostream>
#include <cmath>

Shield::Shield(int centerX, int centerY, int durationMs, int radius, GameField* field) 
    : m_centerX(centerX), m_centerY(centerY), m_durationMs(durationMs), 
      m_radius(radius), m_field(field) {
    
    m_creationTime = std::chrono::steady_clock::now();
    
    for (int x = centerX - radius; x <= centerX + radius; ++x) {
        for (int y = centerY - radius; y <= centerY + radius; ++y) {
            int dx = x - centerX; int dy = y - centerY;
            float dist = std::sqrt(dx*dx + dy*dy);

            if (dist >= radius - 0.8f && dist <= radius + 0.8f) {
                if (m_field && m_field->isInBounds(x, y)) {
                    if (m_field->getCell(x, y).getType() == Cell::EMPTY) {
                        m_blocks.emplace_back(x, y);
                        m_field->getCell(x, y).setType(Cell::OBSTACLE); // Solid for Enemies!
                    }
                }
            }
        }
    }
}

Shield::~Shield() {
    if (m_field) {
        for (const auto& b : m_blocks) {
            if (m_field->isInBounds(b.first, b.second)) {
                // Restore to empty
                m_field->getCell(b.first, b.second).setType(Cell::EMPTY);
            }
        }
    }
}

bool Shield::isActive() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - m_creationTime);
    return elapsed.count() < m_durationMs;
}

bool Shield::contains(int x, int y) const {
    for (const auto& block : m_blocks) {
        if (block.first == x && block.second == y) return true;
    }
    return false;
}

const std::vector<std::pair<int, int>>& Shield::getBlocks() const {
    return m_blocks;
}

void Shield::update() {}

int Shield::getRemainingTimeMs() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - m_creationTime);
    return std::max(0, m_durationMs - static_cast<int>(elapsed.count()));
}