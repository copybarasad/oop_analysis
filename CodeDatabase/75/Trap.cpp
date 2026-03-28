#include "Trap.h"
#include "GameField.h"
#include <iostream>

Trap::Trap(int x, int y, int damage, int durationMs)
    : Character(x, y, TRAP_SYMBOL, "Trap", TRAP_BASE_HEALTH, TRAP_MAX_MANA),
      m_field(nullptr), m_damage(damage), m_durationMs(durationMs), m_triggered(false)
{
    m_creationTime = std::chrono::steady_clock::now();
}

void Trap::update(float dt) {
    Character::update(dt);
    if (m_triggered) return;
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_creationTime);
    if (elapsed.count() >= m_durationMs) { m_triggered = true; }
}

bool Trap::isActive() const { return !m_triggered; }
void Trap::trigger() { if (!m_triggered) { m_triggered = true; } }
int Trap::getRemainingTimeMs() const {
    if (m_triggered) return 0;
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_creationTime);
    int remaining = m_durationMs - static_cast<int>(elapsed.count());
    return std::max(0, remaining);
}