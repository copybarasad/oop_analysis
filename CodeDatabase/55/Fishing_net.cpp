#include "Fishing_net.h"
#include "Player.h"
#include "Sea.h"
#include <iostream>
using namespace std;


Fishing_net::Fishing_net(GameConfig &cfg) : config(cfg), damage(cfg.fish_net_damage), cost(cfg.fish_net_cost),
                                            range(cfg.fish_net_range), flag(SpellFlag::fish_net) {}

Position& Fishing_net::getPosition(){
    return pos;
}

void Fishing_net::setPosition(Position &new_pos){
    if(pos.x < 0 || pos.y < 0 || pos.x >= config.sea_width || pos.y >= config.sea_height){
        cout << "\033[41mНеверная позиция для размещения заклинания!\033[0m" << endl;
        return;
    }
    pos = new_pos;
}

int Fishing_net::getDamage(){
    return damage;
}

void Fishing_net::setDamage(int amount){
    damage = amount;
}

int Fishing_net::getRange(){
    return range;
}

void Fishing_net::setRange(int amount){
    range = amount;
}

SpellFlag Fishing_net::getFlag(){
    return flag;
}

std::string Fishing_net::getFlagString() const{
    return "Заклинание рыболовной сети";
}

int Fishing_net::getCost(){
    return cost;
}

void Fishing_net::setCost(int amount){
    cost = amount;
}

bool Fishing_net::setTrap(Player &pl, Position &new_pos, Sea &sea){
    if(sea.isEmptyPosition(new_pos) && pl.spendScoreOnSpell(cost) && abs(new_pos.x - pl.getPosition().x) <= range && abs(new_pos.y - pl.getPosition().y) <= range){
        setPosition(new_pos);
        sea.setTrapOnSeaZone(new_pos);
        return true;
    }else{
        cout << "\033[41mНевозможно установить ловушку!\033[0m" << endl;
    }
    return false;
}

void Fishing_net::saveTo(SaveFileRAII &out) const{
    config.saveTo(out);
    out.write(damage).write(range).write(cost);
    pos.saveTo(out);
    out.write(static_cast<int>(flag));
}

void Fishing_net::loadFrom(SaveFileRAII& in) {
    config.loadFrom(in);
    in.read(damage).read(range).read(cost);
    pos.loadFrom(in);
    int flag_val; in.read(flag_val);
    flag = static_cast<SpellFlag>(flag_val);
}