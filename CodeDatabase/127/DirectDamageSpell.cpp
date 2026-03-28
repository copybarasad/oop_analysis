// DirectDamageSpell.cpp
#include "DirectDamageSpell.h"
#include "Game.h"
#include <algorithm>
#include <string> 
#include <iostream> 

DirectDamageSpell::DirectDamageSpell(int baseDamage, int range, int price)
    : baseDamage_(baseDamage), range_(range), price_(price) {
}

int DirectDamageSpell::cost() const { return price_; }

bool DirectDamageSpell::apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage) {
    int px = player.getX();
    int py = player.getY();
    int effectiveRange = range_ + enh.extraRange;
    int dist = std::max(std::abs(tx - px), std::abs(ty - py));
    if (dist > effectiveRange) {
        if (errorMessage) *errorMessage = "Цель находится вне радиуса действия заклинания (" + std::to_string(effectiveRange) + ").";
        return false;
    }
    int eidx = game.findEnemyIndexAt(tx, ty);
    if (eidx != -1) {
        int dmg = baseDamage_ + enh.extraDamage;
        game.damageEnemy(eidx, dmg);
        return true;
    }
    int tidx = game.findTowerIndexAt(tx, ty);
    if (tidx != -1) {
        int dmg = baseDamage_ + enh.extraDamage;
        game.damageTower(tidx, dmg);
        return true;
    }
    if (errorMessage) *errorMessage = "На клетке (" + std::to_string(tx) + ", " + std::to_string(ty) + ") нет врага или башни.";
    return false;
}

std::string DirectDamageSpell::getDescription() const {
    return "Наносит " + std::to_string(baseDamage_) + " урона цели в радиусе " + std::to_string(range_) + ". Требует: враг или башня.";
}