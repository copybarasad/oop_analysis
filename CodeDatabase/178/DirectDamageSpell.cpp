#include "DirectDamageSpell.h"
#include "GameController.h"
#include <iostream>
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int dmg, int rng) : damage(dmg), range(rng) {}

bool DirectDamageSpell::apply(GameController &controller, const Position &casterPosition, const Position &targetPos)
{
    int dist = std::abs(casterPosition.x - targetPos.x) + std::abs(casterPosition.y - targetPos.y);
    if (dist > range) {
        std::cout << "DirectDamage: target out of range.\n";
        return false;
    }
    Enemy *e = controller.getEnemyAtPosition(targetPos);
    if (e != nullptr && e->isAlive()) {
        e->takeDamage(damage);
        std::cout << "DirectDamage: dealt " << damage << " to enemy at ("<<targetPos.x<<","<<targetPos.y<<")\n";
        if (!e->isAlive()) {
            std::cout << "Enemy defeated! +20 coins\n";
            controller.getPlayerMutable().addScore(20);
            controller.getPlayerMutable().addCoins(20);
            controller.removeDeadEnemies();
        }
        return true;
    }
    EnemyTower *tw = controller.getTowerAtPosition(targetPos);
    if (tw != nullptr && tw->isAlive()) {
        tw->takeDamage(damage);
        std::cout << "DirectDamage: dealt " << damage << " to tower at ("<<targetPos.x<<","<<targetPos.y<<")\n";
        if (!tw->isAlive()) {
            std::cout << "Tower destroyed! +20 coins\n";
            controller.getPlayerMutable().addScore(20);
            controller.getPlayerMutable().addCoins(20);
        }
        return true;
    }
    std::cout << "DirectDamage: no valid enemy at that position.\n";
    return false;
}