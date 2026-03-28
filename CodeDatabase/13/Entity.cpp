#include "Entity.h"
#include "GameLogic.h"

Entity::~Entity() {
    for (auto *w: weapons_)
        delete w;
}

void Entity::onHit(const int damage) {
    this->hp_ -= damage;
    if (this->hp_ <= 0) {
        this->onDeath();
        this->hp_ = 0;
    }
}

int Entity::attack(Object *target) {
    return this->weapons_.front()->hit(target);
}


void Entity::addStun(const int turns) {
    this->stuned_for_ += turns;
}

int Entity::getStun() const {
    return this->stuned_for_;
}

void Entity::changeWeapon() {
    if (this->getStun() <= 0) {
        this->weapons_.push_back(this->weapons_.front());
        this->weapons_.erase(this->weapons_.begin());
    } else {
        this->addStun(-1);
    }
}

void Entity::addWeapon(Weapon *weapon) {
    this->weapons_.emplace(this->weapons_.begin(), weapon);
}

void Entity::deleteWeapon() {
    this->weapons_.erase(this->weapons_.begin());
}


Weapon *Entity::getWeapon() const {
    return this->weapons_.front();
}

void Entity::onDeath() {
    this->getGameLogic()->addScore(this->points_);
    this->getGameLogic()->removeObject(this->getId());
}

int Entity::move(const int dx, const int dy) {
    if (this->getStun() <= 0) {
        Coordinates to = this->getPosition();
        to.x_ += dx;
        to.y_ += dy;
        return this->getMap()->moveFromTo(this->getPosition(), to);
    }
    this->addStun(-1);
    return 1;
}

int Entity::getHp() const {
    return this->hp_;
}

void Entity::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::saveToJson(savefile);

    (*savefile)["hp"] = hp_;
    (*savefile)["stun"] = stuned_for_;
}

void Entity::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Object::loadFromJson(savefile);

    hp_ = savefile->value("hp", hp_);
    stuned_for_ = savefile->value("stun", stuned_for_);
}
