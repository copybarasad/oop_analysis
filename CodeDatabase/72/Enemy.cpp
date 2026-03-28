//
// Created by bob on 10/4/25.
//

#include "Enemy.h"


Enemy::Enemy(const int &damage, const Position &position, const int &health): Character(damage, position, health) {}


void Enemy::load(Reader &reader) {
    reader.read(damage);
    reader.read(health);
    position.load(reader);
}

void Enemy::save(Saver &saver) const {
    saver.save(damage);
    saver.save(health);
    position.save(saver);
}

