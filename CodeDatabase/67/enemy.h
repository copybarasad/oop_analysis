#ifndef ENEMY_H
#define ENEMY_H
#include "entity.h"
class game_field;

class Enemy : public Entity{
public:
    Enemy(int x, int y);
    void set_position(int x, int y, game_field* field) override;
    char identification() const override;
};

#endif
