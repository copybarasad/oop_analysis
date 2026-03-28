#include "DDSpell.h"

DDSpell::DDSpell(){
    type = "DD";
    damage = DD_SPELL::DD_DAMAGE; //попозже вынести константы в отдельную штуку
    range = DD_SPELL::DD_RANGE;
}

std::vector<Point> DDSpell::get_dests(Point player_coords, char c){
    std::vector<Point> directs;
    for(int i = 1; i < range + 1; i++){
        Point direct(0,0);
        switch(c){
            case CONTROL::DIR_SPELL_UP:
                direct.y -= i;
                break;
            case CONTROL::DIR_SPELL_DOWN:
                direct.y += i;
                break;
            case CONTROL::DIR_SPELL_LEFT:
                direct.x -= i;
                break;
            case CONTROL::DIR_SPELL_RIGHT:
                direct.x += i;
                break;
        }
        directs.push_back(direct);
    }

    std::vector<Point> dests;
    for(int i = 0; i < range; i++){
        Point dest(0,0);
        dest.x = player_coords.x + directs[i].x;
        dest.y = player_coords.y + directs[i].y;
        dests.push_back(dest);
    }

    return dests;
}

int DDSpell::get_damage(){
    return damage;
}

int DDSpell::get_range(){
    return range;
}

void DDSpell::set_damage(int dmg){
    damage = dmg;
}

std::string DDSpell::get_type(){
    return type;
}

bool DDSpell::need_to_return(){
    return true;
}

bool DDSpell::draw(){
    return false;
}


DDSpell::~DDSpell() {}
