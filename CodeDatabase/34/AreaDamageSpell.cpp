#include "AreaDamageSpell.h"
#include "game/Game.h"
#include <iostream>
#include <string>

namespace Game {

AreaDamageSpell::AreaDamageSpell(int damage, int baseAreaSize) 
    : damage(damage), baseAreaSize(baseAreaSize) {}

std::string AreaDamageSpell::getName() const { return "Area Damage"; }
std::string AreaDamageSpell::getDescription() const { return "Damages all enemies in a square area."; }
bool AreaDamageSpell::requiresTarget() const { return true; }

bool AreaDamageSpell::cast(Game& game, const Utils::Point& target, int enhancementStacks) {
    Player& player = game.getPlayer();
    int currentAreaSize = baseAreaSize + enhancementStacks;
    std::cout << "Casting area damage in a " << currentAreaSize << "x" << currentAreaSize << " area around (" 
              << target.row << ", " << target.column << ")." << std::endl;

    for (int r = target.row; r < target.row + currentAreaSize; ++r) {
        for (int c = target.column; c < target.column + currentAreaSize; ++c) {
            Utils::Point currentPoint(r, c);
            if (!game.getMap().isValidPosition(currentPoint)) continue;

            if (auto* enemy = game.getEnemyAtPosition(currentPoint)) {
                bool wasAlive = enemy->isAlive();
                enemy->takeDamage(damage);
                std::cout << "Dealt " << damage << " damage to enemy at (" << r << ", " << c << ")." << std::endl;
                
                if (wasAlive && !enemy->isAlive()) {
                    std::cout << "Enemy defeated by area spell!" << std::endl;
                    player.addScore(100);
                    player.onEnemyKilled();
                }
            }
            if (auto* tower = game.getTowerAtPosition(currentPoint)) {
                tower->takeDamage(damage);
                 std::cout << "Dealt " << damage << " damage to tower at (" << r << ", " << c << ")." << std::endl;
            }
        }
    }
    return true;
}

SpellType AreaDamageSpell::getType() const {
    return SpellType::AreaDamage;
}

void AreaDamageSpell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*>(&baseAreaSize), sizeof(baseAreaSize));
}

void AreaDamageSpell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    is.read(reinterpret_cast<char*>(&baseAreaSize), sizeof(baseAreaSize));
}

}