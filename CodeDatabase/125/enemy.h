#pragma once
#include "position.h"
#include <vector>

class Player;
class GameField;

class Enemy {
public:
    Enemy(Position pos);
    Enemy(Position pos, int hp, int damage); 
    
    Position GetPosition() const;
    void SetPosition(const Position& pos);
    
    int GetHealth() const;
    void TakeDamage(int dmg);
    bool IsAlive() const;
    int GetDamage() const;

    void MoveTowards(Player& player, GameField& field, const std::vector<Enemy>& others);

    void SetHealth(int health) { health_ = health; }
    void SetDamage(int damage) { damage_ = damage; }

private:
    Position position_;
    int health_;
    int damage_;
};