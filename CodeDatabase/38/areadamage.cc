#include "areadamage.h"

bool AreaDamage::doDamage(Player &p, std::vector<Enemy> &enemies, Field &f) {
        int px = p.getX();
        int py = p.getY();
        bool hit = false;

        for (auto &enemy : enemies) {
            int ex = enemy.getX();
            int ey = enemy.getY();

            if (std::abs(ex - px) <= 1 && std::abs(ey - py) <= 1) {
                enemy.takeDamage(damage);
                hit = true;
            }
        }

        // Always cast successfully, even if no one is hit
        return true;
    
    }