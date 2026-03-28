// ThornsArea.h
#pragma once
#include <chrono>
#include <vector>
#include <utility>

class Game;
class Enemy;

class ThornsArea {
public:
    static constexpr int DAMAGE_INTERVAL_SECONDS = 1;
    
    ThornsArea(int centerX, int centerY, int radius, int durationMs, int damagePerSecond);
    
    void update(Game* game);
    bool isActive() const;
    bool affectsPosition(int x, int y) const;
    void applyDamageToEnemies(Game* game);
    const std::vector<std::pair<int, int>>& getAffectedPositions() const { return m_affectedPositions; }
    
    int getRemainingTimeMs() const;
    
private:
    int m_centerX;
    int m_centerY;
    int m_radius;
    int m_damagePerSecond;
    std::chrono::steady_clock::time_point m_endTime;
    std::vector<std::pair<int, int>> m_affectedPositions;
    
    void calculateAffectedPositions();
};