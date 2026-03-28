#include "player.h"
#include "spell.h"
#include "lightning_spell.h"
#include "fireball_spell.h"
#include "trap_spell.h"

Player::Player(): Entity(300,50,0,0), enemies_killed(0)
{    
    hand = new Hand(3);
    hand->addSpell(new LightningSpell());
    hand->addSpell(new FireballSpell());
    hand->addSpell(new TrapSpell());
}

Player::~Player() {
    if (hand != nullptr) {
        delete hand;
        hand = nullptr;
    }
}

void Player::set_position(int x, int y, game_field* field){
    if(field->isValidPosition(x,y)){
        field->get_cell(position_x, position_y).setPlayer(false);
        position_x = x;
        position_y = y;
    }

    if (field->isValidPosition(position_x, position_y)) {
        field->get_cell(position_x, position_y).setPlayer(true);
    }
}

char Player::identification() const {
    return 'P';
}

int Player::get_enemies_killed(){
    return enemies_killed;
}

Hand* Player::get_hand(){
    return hand;
}

void Player::increment_kills(){
    enemies_killed++;
}
