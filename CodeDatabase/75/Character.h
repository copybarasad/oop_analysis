#pragma once
#include <string>
#include <chrono>
#include <map>

class Character {
public:
    enum class Status { 
        NONE, BURNING, FROZEN, STUNNED, POISONED, IN_THORNS, HIT_ANIMATION  
    };
    
    Character(int startX, int startY, char symbol, const std::string& name, int maxHealth, int maxMana = 100);
    virtual ~Character() = default;

    // Logical Coordinates (Grid)
    int getX() const;
    int getY() const;
    
    // Visual Coordinates (Screen Pixels)
    float getVisualX() const { return m_visualX; }
    float getVisualY() const { return m_visualY; }

    virtual int getSize() const { return 1; }
    
    char getSymbol() const;
    const std::string& getName() const;
    bool isAlive() const;
    int getMaxHealth() const;
    int getHealth() const;
    int getMaxMana() const;
    int getMana() const;
    
    virtual void getDamaged(int damage);
    void heal(int amount);
    void useMana(int amount);
    void restoreMana(int amount);
    
    // MOVEMENT:
    void setPosition(int newX, int newY); // Smooth slide
    void teleport(int newX, int newY);    // Instant snap

    void applyStatus(Status status, int durationMs);
    char getDisplaySymbol() const;
    std::string getStatusName() const;
    std::string getStatusDescription() const;
    bool hasStatus(Status status) const;
    
    void updateStatusEffects();
    virtual void update(float dt); 
    
    virtual bool canMoveTo([[maybe_unused]] int x, [[maybe_unused]] int y) const { return true; }

protected:
    static constexpr int HIT_ANIMATION_DURATION_MS = 500;
    static constexpr int BURNING_DAMAGE = 5;
    static constexpr int BURNING_INTERVAL_MS = 1000;
    static constexpr int POISON_DAMAGE = 8;
    static constexpr int POISON_INTERVAL_MS = 2000;
    static constexpr int THORNS_DAMAGE = 3;

    int m_x;
    int m_y;
    
    float m_visualX;
    float m_visualY;
    
    char m_symbol;
    std::string m_name;
    int m_health;
    int m_maxHealth;
    int m_mana;
    int m_maxMana;
    
    // Invulnerability Timer
    float m_invulnTimer; 
    
    std::map<Status, std::chrono::steady_clock::time_point> m_activeStatuses;
    std::map<Status, int> m_statusDurations;
    
    virtual void onStatusApplied(Status status);
    virtual void onStatusEffectTick(Status status);
private:
    void processDamageOverTimeEffects();
};