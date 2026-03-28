#include "EnemyBase.h"
#include  "../Entities/Enemy.h"

#include "../basics/GameLogic.h"
#include "../basics/Map.h"

void EnemyBase::spawnEnemy() {
    Coordinates pos = this->getPosition();
    pos.y_++;
    if (!this->getMap()->isOccupied(pos)) {
        this->getGameLogic()->addObject(new Enemy(), pos);
    } else {
        this->counter_ = 1;
    }
}


void EnemyBase::update() {
    this->counter_--;
    if (this->counter_ == 0) {
        this->counter_ = this->cooldown_;
        this->spawnEnemy();
    }
}

void EnemyBase::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::saveToJson(savefile);

    (*savefile)["cooldown"] = cooldown_;
    (*savefile)["counter"] = counter_;
}

void EnemyBase::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::loadFromJson(savefile);

    cooldown_ = savefile->value("cooldown", cooldown_);
    counter_ = savefile->value("counter", counter_);
}
