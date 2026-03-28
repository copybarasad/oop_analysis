#include "ally.h"

Ally::Ally(unsigned int game_lvl, unsigned int hard_lvl): Character(game_lvl, hard_lvl){
    if(game_lvl < 1) game_lvl = 1;
    if(game_lvl > 3) game_lvl = 3;
    
    type = character_type::ally;    // 1, 2, 3
    hp = (7 + 3 * game_lvl) / 2;    // 5, 6, 8
    melee_damage = 1 + game_lvl;    // 2, 3, 4
    move = true;
};

Ally::Ally(const Ally& ally): Character(ally), melee_damage(ally.melee_damage) {};

Ally::Ally(unsigned int game_lvl, unsigned int hard_lvl, unsigned int hp, bool move): 
        Ally(game_lvl, hard_lvl) {
    this->hp = hp;
    this->move = move;
};

Ally& Ally::operator= (const  Ally& ally){
    type = ally.type;
    hp = ally.hp;
    melee_damage = ally.melee_damage;
    move = ally.move;
    return *this;
};

unsigned int Ally::attack(){
    return melee_damage;
};

unsigned int Ally::range(){
    return 1;
};

void Ally::change_move(){
    move = !move;
};