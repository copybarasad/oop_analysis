#include "Character.h"
#include <iostream>
#include <cmath>

constexpr int Character::BURNING_DAMAGE;
constexpr int Character::BURNING_INTERVAL_MS;
constexpr int Character::POISON_DAMAGE;
constexpr int Character::POISON_INTERVAL_MS;
constexpr int Character::THORNS_DAMAGE;
constexpr int Character::HIT_ANIMATION_DURATION_MS;

Character::Character(int startX, int startY, char symbol, const std::string& name, int maxHealth, int maxMana)
    : m_x(startX), m_y(startY), 
      m_visualX(startX * 48.0f), m_visualY(startY * 48.0f), 
      m_symbol(symbol), m_name(name), 
      m_health(maxHealth), m_maxHealth(maxHealth), m_mana(maxMana), m_maxMana(maxMana),
      m_invulnTimer(0.0f) {}

void Character::update(float dt) {
    float targetX = m_x * 48.0f;
    float targetY = m_y * 48.0f;
    float speed = 12.0f; // FIX: Super Smooth 
    
    m_visualX += (targetX - m_visualX) * speed * dt;
    m_visualY += (targetY - m_visualY) * speed * dt;
    
    if (std::abs(targetX - m_visualX) < 0.5f) m_visualX = targetX;
    if (std::abs(targetY - m_visualY) < 0.5f) m_visualY = targetY;

    if (m_invulnTimer > 0.0f) m_invulnTimer -= dt;
}

void Character::setPosition(int newX, int newY) {
    m_x = newX; m_y = newY;
}

void Character::teleport(int newX, int newY) {
    m_x = newX; m_y = newY;
    m_visualX = newX * 48.0f; m_visualY = newY * 48.0f;
}

void Character::getDamaged(int damage) {
    if (m_invulnTimer > 0.0f) return;
    
    m_health -= damage;
    applyStatus(Status::HIT_ANIMATION, HIT_ANIMATION_DURATION_MS);
    if (m_health < 0) m_health = 0;
    
    // INCREASED I-FRAME DURATION: 1.0s
    m_invulnTimer = 1.0f; 
}

void Character::heal(int amount) {
    m_health += amount; if (m_health > m_maxHealth) m_health = m_maxHealth;
}
void Character::useMana(int amount) {
    m_mana -= amount; if (m_mana < 0) m_mana = 0;
}
void Character::restoreMana(int amount) {
    m_mana += amount; if (m_mana > m_maxMana) m_mana = m_maxMana;
}
void Character::applyStatus(Status status, int durationMs) {
    auto endTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(durationMs);
    m_activeStatuses[status] = endTime;
    m_statusDurations[status] = durationMs;
    onStatusApplied(status);
}
char Character::getDisplaySymbol() const {
    if (hasStatus(Status::HIT_ANIMATION)) return 'X';
    return m_symbol;
}
std::string Character::getStatusName() const { return "NORMAL"; }
std::string Character::getStatusDescription() const { return "No effects"; }
bool Character::hasStatus(Status status) const { return m_activeStatuses.count(status) > 0; }

void Character::updateStatusEffects() {
    auto now = std::chrono::steady_clock::now();
    for (auto it = m_activeStatuses.begin(); it != m_activeStatuses.end(); ) {
        if (now >= it->second) it = m_activeStatuses.erase(it); else ++it;
    }
    processDamageOverTimeEffects();
}

void Character::processDamageOverTimeEffects() {
    auto now = std::chrono::steady_clock::now();
    
    if (hasStatus(Status::BURNING)) {
        static auto lastBurnTime = std::chrono::steady_clock::now();
        if (now - lastBurnTime >= std::chrono::milliseconds(BURNING_INTERVAL_MS)) {
            if(m_invulnTimer <= 0) { m_health -= BURNING_DAMAGE; if(m_health < 0) m_health = 0; }
            onStatusEffectTick(Status::BURNING);
            lastBurnTime = now;
        }
    }
    if (hasStatus(Status::POISONED)) {
        static auto lastPoisonTime = std::chrono::steady_clock::now();
        if (now - lastPoisonTime >= std::chrono::milliseconds(POISON_INTERVAL_MS)) {
            if(m_invulnTimer <= 0) { m_health -= POISON_DAMAGE; if(m_health < 0) m_health = 0; }
            onStatusEffectTick(Status::POISONED);
            lastPoisonTime = now;
        }
    }
}

void Character::onStatusApplied(Status) {}
void Character::onStatusEffectTick(Status) {}
int Character::getX() const { return m_x; }
int Character::getY() const { return m_y; }
char Character::getSymbol() const { return m_symbol; }
const std::string& Character::getName() const { return m_name; }
bool Character::isAlive() const { return m_health > 0; }
int Character::getMaxHealth() const { return m_maxHealth; }
int Character::getHealth() const { return m_health; }
int Character::getMaxMana() const { return m_maxMana; }
int Character::getMana() const { return m_mana; }