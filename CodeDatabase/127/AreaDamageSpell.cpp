// AreaDamageSpell.cpp
#include "AreaDamageSpell.h"
#include "Game.h"
#include <algorithm>
#include <string> 
#include <iostream> 

AreaDamageSpell::AreaDamageSpell(int baseDamage, int area, int price)
    : baseDamage_(baseDamage), area_(area), price_(price) {
}

int AreaDamageSpell::cost() const { return price_; }

bool AreaDamageSpell::apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage) {
    int px = player.getX();
    int py = player.getY();
    int effectiveCastRange = 3 + enh.extraRange;
    int distToCenter = std::max(std::abs(tx - px), std::abs(ty - py));
    if (distToCenter > effectiveCastRange) {
        if (errorMessage) *errorMessage = "Центр области находится вне радиуса действия заклинания (" + std::to_string(effectiveCastRange) + ").";
        return false;
    }
    int side = area_ + enh.extraArea;
    if (side < 1) side = 1;
    int half = side / 2;
    int totalApplied = 0;
    for (int dy = 0; dy < side; ++dy) {
        for (int dx = 0; dx < side; ++dx) {
            int cx = tx + dx - half;
            int cy = ty + dy - half;
            if (!game.getField().isInside(cx, cy)) continue;
            int eidx = game.findEnemyIndexAt(cx, cy);
            if (eidx != -1) {
                int dmg = baseDamage_ + enh.extraDamage;
                game.damageEnemy(eidx, dmg);
                ++totalApplied;
            }
            int tidx = game.findTowerIndexAt(cx, cy);
            if (tidx != -1) {
                int dmg = baseDamage_ + enh.extraDamage;
                game.damageTower(tidx, dmg);
            }
        }
    }
    (void)totalApplied;
    return true;
}

std::string AreaDamageSpell::getDescription() const {
    return "Наносит " + std::to_string(baseDamage_) + " урона всем целям в квадрате " + std::to_string(area_) + "x" + std::to_string(area_) + ". Используется всегда. Радиус применения: 3.";
}