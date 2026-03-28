#include "Mine_spell.h"
#include "Player.h"
#include "Sea.h"
#include "Game.h"
#include <iostream>
#include <algorithm>
using namespace std;


Mine_spell::Mine_spell(GameConfig &cfg) : config(cfg), damage(cfg.mine_spell_damage), range(cfg.mine_spell_range),
                                cost(cfg.mine_spell_cost), flag(mine_flag){}

int Mine_spell::getDamage(){
    return damage;
}

void Mine_spell::setDamage(int amount){
    damage = amount;
}

int Mine_spell::getRange(){
    return range;
}

void Mine_spell::setRange(int amount){
    range = amount;
}

int Mine_spell::getCost(){
    return cost;
}

void Mine_spell::setCost(int amount){
    cost = amount;
}

SpellFlag Mine_spell::getSpellType(){
    return flag;
}

std::string Mine_spell::getSpellTypeString() const {
    return "Заклинание подводной мины";
}

bool Mine_spell::attack(Player& pl, Sea& sea, std::vector<Enemy>& enemies, std::vector<Octopus>& octopuses, const Position& center){
    Position playerPos = pl.getPosition();
    int dx = abs(center.x - playerPos.x);
    int dy = abs(center.y - playerPos.y);

    if (dx > range || dy > range) {
        return false;
    }

    for (int y = center.y; y <= center.y + 1; ++y) {
        for (int x = center.x; x <= center.x + 1; ++x) {
            Position pos(x, y);
            if (!sea.isValidPosition(pos)) continue;

            if (sea.getSeaZone(pos).getFlag() == enemy_flag) {
                for (auto& enemy : enemies) {
                    if (enemy.getPosition() == pos && enemy.isAlive()) {
                        enemy.takeDamage(damage);
                        pl.addScore();
                        pl.getBag().gift();
                    }
                }
            }else if(sea.getSeaZone(pos).getFlag() == octopus_flag){
                for (auto& oct : octopuses) {
                    if (oct.getPosition() == pos && oct.isAlive()) {
                        oct.takeDamage(damage);
                        pl.addScore();
                        pl.getBag().gift();
                    }
                }
            }
        }
    }
    return true;
}

void Mine_spell::saveTo(SaveFileRAII &out) const{
    config.saveTo(out);
    out.write(damage).write(range).write(cost);
    out.write(static_cast<int>(flag));
}

void Mine_spell::loadFrom(SaveFileRAII& in) {
    config.loadFrom(in);
    in.read(damage).read(range).read(cost);
    int flag_val; in.read(flag_val);
    flag = static_cast<SpellFlag>(flag_val);
}