#include "fireball.h"
#include <iostream>

Fireball::Fireball(int id, const std::string& name, int cost, int damage, int range)
    : Spell(id, name, cost), damage(damage), range(range) {}

void Fireball::apply(Character& caster, Character& target) const {
    if (!target.getisAlive()) return;

    int dx = std::abs(caster.getPosX() - target.getPosX());
    int dy = std::abs(caster.getPosY() - target.getPosY());

    if (dx <= range && dy <= range){
        target.takeDamage(damage);

        std::cout << caster.getName() << " casts " << name << " on "
        << target.getName() << " for " << damage << " damage!\n";
    }else{
        std::cout << "Target is out of range\n";
    }
}


void Fireball::applyArea(Character& caster, int centerX, int centerY, Game& game){
    std:: cout << "This spell requires a target!\n";
}
