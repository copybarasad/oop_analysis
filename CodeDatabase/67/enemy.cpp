#include "enemy.h"
#include "game_field.h"

Enemy::Enemy(int x, int y)
    : Entity(100, 20, x, y)
{}

void Enemy::set_position(int x, int y, game_field* field) {
    if(field->isValidPosition(x,y)){
        field->get_cell(position_x, position_y).setEnemy(false);
        position_x = x;
        position_y = y;
    }

    if (field->isValidPosition(position_x, position_y)) {
        field->get_cell(position_x, position_y).setEnemy(true);
    }
}

char Enemy::identification() const { 
    return 'E';
}
