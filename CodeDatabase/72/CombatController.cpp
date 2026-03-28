//
// Created by bob on 10/15/25.
//

#include "CombatController.h"

#include <iostream>
#include <unistd.h>

CombatController::CombatController(std::shared_ptr<Field> field): field(field) {}

void CombatController::playerShoot(const Direction &direction) {
    Position pos = field->getPlayer()->getPosition();

    switch (direction) {
        case Direction::Left:
            for (int x = pos.getX() - 1; x >= 0; --x)
                if (field->isEnemy(Position{x, pos.getY()}, false) || field->isEnemyBuild(Position{x, pos.getY()}, false))
                    return;
            break;

        case Direction::Right:
            for (int x = pos.getX() + 1; x < field->getWidth(); ++x)
                if (field->isEnemy(Position{x, pos.getY()}, false)|| field->isEnemyBuild(Position{x, pos.getY()}, false))
                    return;
            break;

        case Direction::Up:
            for (int y = pos.getY() - 1; y >= 0; --y)
                if (field->isEnemy(Position{pos.getX(), y}, false) || field->isEnemyBuild(Position{pos.getX(), y}, false))
                    return;
            break;

        case Direction::Down:
            for (int y = pos.getY() + 1; y < field->getHeight(); ++y)
                if (field->isEnemy(Position{pos.getX(), y}, false) || field->isEnemyBuild(Position{pos.getX(), y}, false))
                    return;
            break;
    }

}

