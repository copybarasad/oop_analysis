#include "TrapManager.h"
#include <iostream>
#include <algorithm>

void TrapManager::checkTrapCollisions(std::vector<Trap> &traps, std::vector<Enemy> &enemies, Player &player) {
    for (auto eit = enemies.begin(); eit != enemies.end();) {
        bool enemyRemoved = false;
        for (auto tit = traps.begin(); tit != traps.end();) {
            if (eit->getPosition() == tit->getPosition()) {
                std::cout << "Enemy stepped on trap at (" << tit->getPosition().x << "," << tit->getPosition().y 
                          << ") took " << tit->getDamage() << " damage.\n";
                eit->takeDamage(tit->getDamage());
                tit = traps.erase(tit);
                
                if (!eit->isAlive()) {
                    std::cout << "Enemy killed by trap! +20 coins\n";
                    player.addCoins(20);
                    player.addScore(20);
                    enemyRemoved = true;
                    break;
                }
            } else {
                ++tit;
            }
        }
        
        if (enemyRemoved) {
            eit = enemies.erase(eit);
        } else {
            ++eit;
        }
    }
}

void TrapManager::addTrap(std::vector<Trap> &traps, const Trap &trap) {
    traps.push_back(trap);
}

void TrapManager::removeTrapAt(std::vector<Trap> &traps, const Position &pos) {
    traps.erase(std::remove_if(traps.begin(), traps.end(), 
                [&](const Trap &tr){ return tr.getPosition() == pos; }), traps.end());
}