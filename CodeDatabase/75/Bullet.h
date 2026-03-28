#pragma once

class Bullet {
public:
    enum class Direction { UP, DOWN, LEFT, RIGHT };
    enum class Type { MAGIC_MISSILE, FIREBALL, ARROW, DARK_ORB, AXE, BOSS_BLAST };

    Bullet(int startX, int startY, Direction direction, int damage, Type type = Type::MAGIC_MISSILE);
    ~Bullet() = default;
    
    void update(float dt);
    
    bool isActive() const { return m_active; }
    void deactivate() { m_active = false; }
    
    int getX() const { return m_x; }
    int getY() const { return m_y; }
    int getDamage() const { return m_damage; }
    Type getType() const { return m_type; }
    Direction getDirection() const { return m_direction; }
    
    float getRenderX() const { return m_visualX; } 
    float getRenderY() const { return m_visualY; }
    
private:
    int m_x, m_y;
    float m_visualX, m_visualY;
    Direction m_direction;
    int m_damage;
    bool m_active;
    Type m_type;
    float m_moveTimer; 
    float m_speedDelay; 
};