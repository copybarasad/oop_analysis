#include "Jet_spell.h"
#include "Player.h"
#include "Sea.h"
#include "Game.h"
#include <iostream>
using namespace std;


Jet_spell::Jet_spell(GameConfig &cfg) : config(cfg), damage(cfg.jet_spell_damage), range(cfg.jet_spell_range),
                            cost(cfg.jet_spell_cost), flag(jet_flag){}

int Jet_spell::getDamage(){
    return damage;
}

void Jet_spell::setDamage(int amount){
    damage = amount;
}

int Jet_spell::getRange(){
    return range;
}

void Jet_spell::setRange(int amount){
    range = amount;
}

int Jet_spell::getCost(){
    return cost;
}

void Jet_spell::setCost(int amount){
    cost = amount;
}

SpellFlag Jet_spell::getSpellType(){
    return flag;
}

std::string Jet_spell::getSpellTypeString() const {
    return "Заклинание водного потока";
}

bool Jet_spell::attack(Player& pl, Sea& sea, std::vector<Enemy>& enemies, std::vector<Octopus>& octopuses, const Position& target){
    Position playerPos = pl.getPosition();
    int dx = abs(target.x - playerPos.x);
    int dy = abs(target.y - playerPos.y);

    if (dx > range || dy > range || (dx == 0 && dy == 0)) {
        return false;
    }

    if (sea.getSeaZone(const_cast<Position&>(target)).getFlag() != enemy_flag && sea.getSeaZone(const_cast<Position&>(target)).getFlag() != octopus_flag) {
        return false;
    }

    for (auto& enemy : enemies) {
        if (enemy.getPosition() == target && enemy.isAlive()) {
            enemy.takeDamage(damage);
            pl.addScore();
            pl.getBag().gift();
            return true;
        }
    }
    for (auto& oct : octopuses) {
        if (oct.getPosition() == target && oct.isAlive()) {
            oct.takeDamage(damage);
            pl.addScore();
            pl.getBag().gift();
            return true;
        }
    }
    return false;
}

void Jet_spell::saveTo(SaveFileRAII &out) const{
    config.saveTo(out);
    out.write(damage).write(range).write(cost);
    out.write(static_cast<int>(flag));
}

void Jet_spell::loadFrom(SaveFileRAII& in){
    config.loadFrom(in);
    in.read(damage).read(range).read(cost);
    int flag_val; in.read(flag_val);
    flag = static_cast<SpellFlag>(flag_val);
}