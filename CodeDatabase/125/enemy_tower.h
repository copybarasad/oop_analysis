#pragma once
#include "position.h"
#include <string>

class Player;
class GameField;

class EnemyTower {
public:
    explicit EnemyTower(Position pos, int damage = 15, int range = 4, int cooldown = 2);

    void Update(Player& player, GameField& field);

    const Position& GetPosition() const { return position_; }
    bool IsAlive() const;
    void TakeDamage(int damage);
    int GetHealth() const { return health_; }

    bool CanSeePlayer(const Player& player) const;

    void SetHealth(int health) { health_ = health; }
    void SetPosition(const Position& pos) { position_ = pos; }

private:
    Position position_;
    int health_;
    int damage_;     
    int range_;      
    int cooldown_;   
    int current_cooldown_;  

    void Shoot(Player& player);
};