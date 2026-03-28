#include "AoESpell.h"
#include "Constants.h"
#include "CombatManager.h"

AoESpell::AoESpell(int dmg, int area) : damage(dmg), areaSize(area) {}

bool AoESpell::cast(GameField& field, const Position& center, Player& player, Game& game) {

    CombatManager cm;
    Position playerPos = player.getPosition();

    if (!cm.isInAttackRange(playerPos, center, DEFAULT_SPELL_AOE_RANGE)) {
        std::cout << "Fireball target is out of range (" << DEFAULT_SPELL_AOE_RANGE << ")!" << std::endl;
        return false; 
    }

    int effectiveArea = areaSize + player.getHand().getBuffCount()*areaSize; 

    int x0 = center.getX() - (effectiveArea / 2);
    int y0 = center.getY() - (effectiveArea / 2);

    const auto& enemies = game.getEnemies();
    const auto& buildings = game.getBuildings();

    int hitCount = 0;
    for (int dx = 0; dx < effectiveArea; ++dx) {
        for (int dy = 0; dy < effectiveArea; ++dy) {
            int x = x0 + dx;
            int y = y0 + dy;
            if (field.isValidPosition(x, y)) {
                for (auto& enemy : enemies) {
                    if (enemy && enemy->isAlive() && 
                        enemy->getPosition().getX() == x && 
                        enemy->getPosition().getY() == y) {
                        enemy->takeDamage(damage);
                        std::cout << "Fireball hit enemy at (" << x << "," << y << ") for " 
                        << damage << " damage!" << std::endl;
                        hitCount++;
                    }
                }

                for (auto& building : buildings) {
                    if (building && building->isAlive() && 
                        building->getPosition().getX() == x && 
                        building->getPosition().getY() == y) {
                        building->takeDamage(damage);
                        std::cout << "Fireball hit building at (" << x << "," << y << ") for " 
                        << damage << " damage!" << std::endl;
                        hitCount++;
                    }
                }
            }
        }
    }

    if (hitCount > 0) {
        std::cout << "Fireball hit " << hitCount << " targets!" << std::endl;
    } else {
        std::cout << "Fireball activated, but hit nothing. Are you even trying?" << std::endl;
    }

    return true;
}

std::string AoESpell::getName() const {
    return "Fireball";
}