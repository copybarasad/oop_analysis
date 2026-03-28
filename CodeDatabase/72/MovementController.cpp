//
// Created by bob on 10/15/25.
//

#include "MovementController.h"

MovementController::MovementController(std::shared_ptr<Field> field): field(field) {}

Position MovementController::directionToOffset(const Direction &direction) {
    Position offset;
    switch (direction) {
        case Direction::Down:  offset = {0, 1};  break;
        case Direction::Up:    offset = {0, -1}; break;
        case Direction::Left:  offset = {-1, 0}; break;
        case Direction::Right: offset = {1, 0};  break;
    }
    return offset;
}


bool MovementController::movePlayer(const Direction &direction) {
    Position offset = directionToOffset(direction);
    Position newPos = field->getPlayer()->getPosition() + offset;

    if (!field->inField(newPos) || field->isObstacle(newPos))
        return false;

    if (!field->isEnemy(newPos, false)
        && !field->isEnemyBuild(newPos, false)
        && !field->isTower(newPos, false)) {
        field->getPlayer()->move(offset);
        if (field->isSlow(newPos))
            field->getPlayer()->setSkipStep(true);
    }

    return true;
}

void MovementController::moveEnemies() {

    std::list<Enemy> &enemies = field->getEnemiesMutable();

    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        int value = rand() % 4;
        Position offset;

        switch (value) {
            case 0: offset = {0, 1}; break;
            case 1: offset = {0, -1}; break;
            case 2: offset = {1, 0}; break;
            case 3: offset = {-1, 0}; break;
        }

        Position newPos = it->getPosition() + offset;

        if (field->inField(newPos)
            && !field->isPlayer(newPos, it->getDamage())
            && !field->isObstacle(newPos)
            && !field->isEnemyBuild(newPos, true)
            && !field->isEnemy(newPos, true)
            && !field->isTower(newPos, true)) {
                it->move(offset);
                field->checkTrap(newPos, it);
            }
    }
}

