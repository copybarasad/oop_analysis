#pragma once
#include <chrono>
#include <vector>

class GameField;

class Shield {
private:
    int m_centerX, m_centerY;
    int m_durationMs;
    int m_radius;
    std::chrono::steady_clock::time_point m_creationTime;
    std::vector<std::pair<int, int>> m_blocks;
    GameField* m_field;
    
public:
    static constexpr int BASE_DURATION_MS = 20000;
    static constexpr int DURATION_PER_LEVEL_MS = 5000;
    static constexpr int BASE_RADIUS = 3;  
    static constexpr int RADIUS_PER_LEVEL = 1;
    
    Shield(int centerX, int centerY, int durationMs, int radius, GameField* field);
    ~Shield();
    
    int getCenterX() const { return m_centerX; }
    int getCenterY() const { return m_centerY; }
    int getRadius() const { return m_radius; }
        
    bool isActive() const;
    bool contains(int x, int y) const;
    const std::vector<std::pair<int, int>>& getBlocks() const;
    void update();
    int getRemainingTimeMs() const;
};