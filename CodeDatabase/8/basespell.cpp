#include "basespell.h"
#include "player.h"
#include "field.h"
#include <iostream>
#include <cmath>
#include <algorithm>

BaseSpell::BaseSpell(int dmg, int cost, int radius, std::string spellName) 
    : damage(dmg), cost(cost), range(radius), name(spellName) {}

bool BaseSpell::checkResources(int x, int y, Field& field, Player& player, std::string& message) {
    if (player.getPoints() < cost) {
        message =  "Недостаточно очков! Нужно: " + std::to_string(cost) + ", есть: " + std::to_string(player.getPoints());
        return false;
    }
    int distance = std::max(std::abs(x - player.getX()), std::abs(y - player.getY()));
    if (distance > range) {
        message = "Цель слишком далеко! Дистанция: " + std::to_string(distance) + ", макс: " + std::to_string(range);
        return false;
    }
    if (!field.validpos(x, y)) {
        message = "Неверная позиция!";
        return false;
    }
    return true;
}

void BaseSpell::removeCash(Player& player) {
    player.removePoints(cost);
}

std::string BaseSpell::getName() const { return name; }
int BaseSpell::getCost() const { return cost; }
int BaseSpell::getRange() const { return range; }
int BaseSpell::getDamage() const { return damage; }