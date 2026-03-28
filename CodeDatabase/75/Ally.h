#pragma once
#include "Character.h"
#include <vector>

class GameField;
class Enemy;
class Game;

class Ally : public Character {
public:
    Ally(int startX, int startY, const std::string& name, int maxHealth, int damage);
    ~Ally();
    
    void update(float dt) override;
    
    void updateAI(const std::vector<Enemy*>& enemies, Game* game);
    
    void setGameField(GameField* field);
    int getDamage() const { return m_damage; }
    
private:
    GameField* m_field;
    int m_damage;
    float m_moveTimer;
    float m_attackTimer;
    
    void moveTowards(int targetX, int targetY);
    bool isValidMove(int x, int y) const;
};