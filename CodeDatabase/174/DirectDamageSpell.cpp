#include "DirectDamageSpell.h"
#include "Constants.h"

DirectDamageSpell::DirectDamageSpell(int dmg, int rng) : damage(dmg), range(rng) {}

bool DirectDamageSpell::cast(GameField& field, const Position& targetPos, Player& player, Game& game) {
    int effectiveRange = range + player.getHand().getBuffCount()*range; 

    if (!field.isValidPosition(targetPos)) {
        std::cout << "Invalid target position!" << std::endl;
        return false;
    }

    CellType targetCell = field.getCellType(targetPos.getX(), targetPos.getY());

    if (targetCell != CellType::ENEMY && 
        targetCell != CellType::ENEMY_BARRACK && 
        targetCell != CellType::ENEMY_TOWER) {
        std::cout << "Target is not an enemy or enemy building!" << std::endl;
        return false;
    }

    CombatManager cm;
    Position playerPos = player.getPosition();

    if (!cm.isInAttackRange(playerPos, targetPos, effectiveRange)) {
        std::cout << "Target is out of range!" << std::endl;
        return false;
    }

    const auto& enemies = game.getEnemies();
    const auto& buildings = game.getBuildings();

    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive() && enemy->getPosition() == targetPos) {
            enemy->takeDamage(damage);
            std::cout << "Lightning Bolt dealt " << damage << " to enemy!" << std::endl;
            return true;
        }
    }

    for (auto& building : buildings) {
        if (building && building->isAlive() && building->getPosition() == targetPos) {
            building->takeDamage(damage);
            std::cout << "Lightning Bolt dealt " << damage << " to building!" << std::endl;
            return true;
        }
    }

    std::cout << "Target not found!" << std::endl;
    return false;
}

std::string DirectDamageSpell::getName() const {
    return "Lightning Bolt";
}