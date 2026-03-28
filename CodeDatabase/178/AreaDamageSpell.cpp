#include "AreaDamageSpell.h"
#include "GameController.h"
#include <iostream>
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int dmg, int w, int h, int rng) : 
    damage(dmg), width(w), height(h), range(rng) {}

bool AreaDamageSpell::apply(GameController &controller, const Position &casterPosition, const Position &targetPos)
{
    int dist = std::abs(casterPosition.x - targetPos.x) + std::abs(casterPosition.y - targetPos.y);
    if (dist > range) {
        std::cout << "AreaDamage: target out of range.\n";
        return false;
    }
    int hits = 0;
    for (int dy=0; dy<height; ++dy) {
        for (int dx=0; dx<width; ++dx) {
            Position p(targetPos.x+dx, targetPos.y+dy);
            if (!controller.getField().isValidPosition(p)) continue;
            Enemy *e = controller.getEnemyAtPosition(p);
            if (e != nullptr && e->isAlive()) {
                e->takeDamage(damage);
                ++hits;
                std::cout << "AreaDamage: "<<damage<<" to enemy at ("<<p.x<<","<<p.y<<")\n";
                if (!e->isAlive()) {
                    std::cout << "Enemy defeated! +20 coins\n";
                    controller.getPlayerMutable().addScore(20);
                    controller.getPlayerMutable().addCoins(20);
                }
            }
        }
    }
    std::cout << "AreaDamage: hit "<<hits<<" enemies.\n";
    controller.removeDeadEnemies();
    return true;
}