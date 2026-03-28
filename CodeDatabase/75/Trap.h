#pragma once
#include "Character.h"
#include <chrono>

class GameField;

class Trap : public Character {
private:
    static constexpr int TRAP_BASE_HEALTH = 1;
    static constexpr char TRAP_SYMBOL = '*';
    static constexpr int TRAP_MAX_MANA = 0;
public:
    Trap(int x, int y, int damage, int durationMs);
    ~Trap() = default;
    
    void update(float dt) override; 
    bool isActive() const;
    void trigger();
    bool isTriggered() const { return m_triggered; }
    
    void setGameField(GameField* field) { m_field = field; }
    int getDamage() const { return m_damage; }
    
    int getRemainingTimeMs() const;
    int getDurationMs() const { return m_durationMs; }
    
private:
    GameField* m_field;
    int m_damage;
    int m_durationMs;
    bool m_triggered;
    std::chrono::steady_clock::time_point m_creationTime;
};