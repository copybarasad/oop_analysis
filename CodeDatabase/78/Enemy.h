#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class GameField;
class Player;

class Enemy : public Entity {
public:
    Enemy(int health, int startX, int startY)
        : Entity(health, startX, startY, WeaponType::FISTS) {}
    
    void update(GameField& game_field);
    
    void show_stats() const override;
    
private:
    void move(GameField& game_field, Player& player);
};

#endif