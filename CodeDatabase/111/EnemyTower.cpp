#include "EnemyTower.h"
#include "GameBoard.h"
#include "Hero.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(int hitpoints, int attack, const Location& pos, int range)
    : GameActor(hitpoints, attack, pos),  // Исправлено: передаем полный урон, а не attack / 2
      attackSpell("Tower Bolt", attack, 0, range),  // Исправлено: полный урон
      canAttackThisTurn(true),
      attackRange(range) {
}

void EnemyTower::performMove(MoveDirection dir, GameBoard& board) {
    // Башня неподвижна
}

void EnemyTower::handleCollision(GameActor* other) {
    // Башня не атакует в ближнем бою, только получает урон
    if (other && other->isAlive() && isAlive()) {
        // Герой атакует башню в ближнем бою
        Hero* hero = dynamic_cast<Hero*>(other);
        if (hero) {
            std::cout << "Hero attacks tower with melee! Tower takes damage.\n";
            // Урон будет нанесен через receiveDamage от GameActor
        }
    }
}

void EnemyTower::performTurn(GameBoard& board, const Location& heroPosition) {
    if (canAttackThisTurn && isAlive() && isInAttackRange(heroPosition)) {
        bool hasLineOfSight = true;
        
        int deltaRow = static_cast<int>(heroPosition.row) - static_cast<int>(position.row);
        int deltaCol = static_cast<int>(heroPosition.col) - static_cast<int>(position.col);
        
        int steps = std::max(std::abs(deltaRow), std::abs(deltaCol));
        
        if (steps > 1) {
            for (int i = 1; i < steps; ++i) {
                Location checkPos(
                    position.row + (deltaRow * i) / steps,
                    position.col + (deltaCol * i) / steps
                );
                
                if (board.isObstacle(checkPos)) {
                    hasLineOfSight = false;
                    break;
                }
            }
        }
        
        if (hasLineOfSight && attackSpell.canCast(position, heroPosition, board)) {
            std::cout << "Tower attacks hero with " << attackSpell.getName() << "!\n";
            attackSpell.cast(position, heroPosition, board);
            canAttackThisTurn = false;
        }
    } else {
        canAttackThisTurn = true;
    }
}

bool EnemyTower::isInAttackRange(const Location& target) const {
    int distance = std::abs(static_cast<int>(position.row) - static_cast<int>(target.row)) +
                   std::abs(static_cast<int>(position.col) - static_cast<int>(target.col));
    return distance <= attackRange;
}