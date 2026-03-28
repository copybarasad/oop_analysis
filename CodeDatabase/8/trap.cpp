#include "trap.h"
#include "field.h"
#include <iostream>

TrapSpell::TrapSpell(int dmg, int cost, int radius, std::string spellName) 
    : BaseSpell(dmg, cost, radius, spellName) {}

bool TrapSpell::do_spell(int x, int y, Field& field, Player& player, Enemy& enemy, std::string& message) {
    if (!checkResources(x, y, field, player, message)) return false;
    
    if (!field.validpos(x, y) || !field.getNode(x, y).isEmpty() || !field.getNode(x, y).isPassable()) {
        message = "Невозможно установить ловушку!";
        return false;
    }
    
    field.getNode(x, y).setTrap(damage);
    removeCash(player);
    message = name + " установлена в (" + std::to_string(x) + "," + std::to_string(y) + ")! Нанесет урон: " + std::to_string(damage);
    return true;
}