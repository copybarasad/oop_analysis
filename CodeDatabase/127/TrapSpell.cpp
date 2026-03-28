// TrapSpell.cpp
#include "TrapSpell.h"
#include "Game.h"
#include <string>

TrapSpell::TrapSpell(int damage, int price) : damage_(damage), price_(price) {}

int TrapSpell::cost() const { return price_; }

bool TrapSpell::apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage) {
    int dmg = damage_ + enh.extraDamage;
    return game.placeTrap(tx, ty, dmg);
}

std::string TrapSpell::getDescription() const {
    return "Устанавливает ловушку с уроном " + std::to_string(damage_) + " на клетку (tx, ty). Наносит урон врагу при наступлении.";
}