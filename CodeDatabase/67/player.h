#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "hand.h"

class Player : public Entity{
    int enemies_killed;
    Hand* hand;
    
public:
    Player();
    ~Player();

    int get_enemies_killed();
    Hand* get_hand();
    void set_position(int x, int y, game_field* field) override;
    void increment_kills();
    char identification() const override;
};

#endif
