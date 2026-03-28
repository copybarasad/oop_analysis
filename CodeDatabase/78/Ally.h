#ifndef ALLY_H
#define ALLY_H

#include "Entity.h"

class GameField;

class Ally : public Entity {
public:
    Ally(int health, int x, int y)
        : Entity(health, x, y, WeaponType::FISTS) {}
    
    void update(GameField& game_field);
    void show_stats() const override;
    
private:
    void move_towards_nearest_enemy(GameField& field);
    bool find_nearest_enemy(GameField& field, int& target_x, int& target_y) const;
};

#endif