#include "ThornsArea.h"
#include "Game.h"
#include "Enemy.h"
#include "VisualEffects.h"
#include <iostream>

constexpr int ThornsArea::DAMAGE_INTERVAL_SECONDS;

ThornsArea::ThornsArea(int centerX, int centerY, int radius, int durationMs, int damagePerSecond)
    : m_centerX(centerX), m_centerY(centerY), m_radius(radius),
      m_damagePerSecond(damagePerSecond) {
    m_endTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(durationMs);
    calculateAffectedPositions();
}

int ThornsArea::getRemainingTimeMs() const {
    auto now = std::chrono::steady_clock::now();
    auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - now);
    return std::max(0, static_cast<int>(remaining.count()));
}

void ThornsArea::calculateAffectedPositions() {
    m_affectedPositions.clear();
    for (int x = m_centerX - m_radius; x <= m_centerX + m_radius; ++x) {
        for (int y = m_centerY - m_radius; y <= m_centerY + m_radius; ++y) {
            int dx = x - m_centerX;
            int dy = y - m_centerY;
            if (dx * dx + dy * dy <= m_radius * m_radius) {
                m_affectedPositions.emplace_back(x, y);
            }
        }
    }
}

void ThornsArea::update(Game* game) {
    if (!isActive()) return;
    
    static auto lastDamageTime = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    
    // TICK EVERY 200ms
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastDamageTime).count() >= 200) {
        applyDamageToEnemies(game);
        lastDamageTime = now;
    }
}

bool ThornsArea::isActive() const {
    return std::chrono::steady_clock::now() < m_endTime;
}

bool ThornsArea::affectsPosition(int x, int y) const {
    for (const auto& pos : m_affectedPositions) {
        if (pos.first == x && pos.second == y) return true;
    }
    return false;
}

void ThornsArea::applyDamageToEnemies(Game* game) {
    const auto& enemies = game->getEnemies();
    for (auto* enemy : enemies) {
        if (enemy->isAlive() && affectsPosition(enemy->getX(), enemy->getY())) {
            enemy->getDamaged(m_damagePerSecond);
        }
    }
}