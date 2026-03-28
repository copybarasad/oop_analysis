#pragma once
#include "Character.h"
#include <vector>
#include <string>

class GameField; 
class Bullet;
class Game; // Forward declaration    

class Enemy : public Character {
    friend class Game;
public:
    enum class Type { MELEE, RANGED, FAST, TANK, MAGE, BOSS };

    Enemy(int x, int y, Type type, const std::string& name, int maxHealth, int damage);
    virtual ~Enemy();

    void update(float dt) override; // Updated signature
    
    void setGameField(GameField* field);
    std::string getTypeName() const;
    Type getType() const { return m_type; } 
    int getDamage() const { return m_damage; }

    void shootAtPlayer(int px, int py);
    void moveTowards(int px, int py);

    static std::vector<Enemy*> s_spawnQueue;
    static void spawn(int x, int y, Type type, int difficulty);

protected:
    Type m_type;
    int m_damage;
    GameField* m_field;
    float m_attackTimer;
    float m_moveTimer;
    
    std::vector<Bullet*> m_bullets;
    
public:
    const std::vector<Bullet*>& getBullets() const { return m_bullets; }
    std::vector<Bullet*>& getBullets() { return m_bullets; }
};