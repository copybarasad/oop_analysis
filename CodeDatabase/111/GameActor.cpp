#include "GameActor.h"
#include "GameUtils.h"
#include "MoveDirection.h"

GameActor::GameActor(int hitpoints, int attack, const Location& pos)
    : attributes(hitpoints, attack), position(pos) {
}

void GameActor::receiveDamage(int damage) {
    if (damage > 0 && isAlive()) {
        attributes.takeDamage(damage);
    }
}

Location GameActor::getPosition() const {
    return position;
}

int GameActor::getHitpoints() const {
    return attributes.hitpoints;
}

int GameActor::getMaxHitpoints() const {
    return attributes.getMaxHitpoints();
}

int GameActor::getAttackPower() const {
    return attributes.attackPower;
}

bool GameActor::isAlive() const {
    return attributes.isAlive();
}

void GameActor::setPosition(const Location& newPos) {
    position = newPos;
}