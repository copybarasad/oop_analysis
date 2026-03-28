#include "TrapSpell.h"

TrapSpell::TrapSpell(){
    type = "Trap";
    damage = TRAP_SPELL::TR_DAMAGE;
    range = TRAP_SPELL::TR_RANGE;
}

std::vector<Point> TrapSpell::get_dests(Point player_coords, char c){
    std::vector<Point> dests;
    dests.push_back(player_coords);
    return dests;
}

int TrapSpell::get_damage(){
    return damage;
}

int TrapSpell::get_range(){
    return range;
}

std::string TrapSpell::get_type(){
    return type;
}

bool TrapSpell::need_to_return(){
    return false; //потому что по любому оно не сработает сразу
}

bool TrapSpell::draw(){
    return true;
}

TrapSpell::~TrapSpell() {}
