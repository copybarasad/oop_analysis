#include "ARSpell.h"

ARSpell::ARSpell(){
    type = "Area";
    damage = AREA_SPELL::AR_DAMAGE;
    range = AREA_SPELL::AR_RANGE; 
}

std::vector<Point> ARSpell::get_dests(Point player_coords, char c){
    std::vector<Point> dests;
    switch(c){
        case CONTROL::DIR_SPELL_UP:
            dests.push_back(Point(player_coords.x - 1, player_coords.y));
            dests.push_back(Point(player_coords.x - 1, player_coords.y - 1));
            dests.push_back(Point(player_coords.x, player_coords.y - 1));
            break;
        case CONTROL::DIR_SPELL_LEFT:
            dests.push_back(Point(player_coords.x - 1, player_coords.y));
            dests.push_back(Point(player_coords.x - 1, player_coords.y + 1));
            dests.push_back(Point(player_coords.x, player_coords.y + 1));
            break;
        case CONTROL::DIR_SPELL_DOWN:
            dests.push_back(Point(player_coords.x + 1, player_coords.y));
            dests.push_back(Point(player_coords.x + 1, player_coords.y + 1));
            dests.push_back(Point(player_coords.x, player_coords.y + 1));
            break;
        case CONTROL::DIR_SPELL_RIGHT:
            dests.push_back(Point(player_coords.x, player_coords.y - 1));
            dests.push_back(Point(player_coords.x + 1, player_coords.y));
            dests.push_back(Point(player_coords.x + 1, player_coords.y - 1));
            break;
    }

    return dests;
}

int ARSpell::get_damage(){
    return damage;
}

int ARSpell::get_range(){
    return range;
}

std::string ARSpell::get_type(){
    return type;
}

bool ARSpell::need_to_return(){
    return false;
}

bool ARSpell::draw(){
    return false;
}

ARSpell::~ARSpell() {}

