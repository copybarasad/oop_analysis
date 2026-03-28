#include "Enemy.h"
#include <random>
#include "../basics/Map.h"

void Enemy::update() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dir_dist(0, 3);
    int dir = dir_dist(gen);
    int stuck = 0;
    Coordinates pos = this->getPosition();

    switch (dir) {
        case 0:
            stuck = this->move(1, 0);
            pos.x_++;
            break;
        case 1:
            stuck = this->move(-1, 0);
            pos.x_--;
            break;
        case 2:
            stuck = this->move(0, 1);
            pos.y_++;
            break;
        case 3:
            stuck = this->move(0, -1);
            pos.y_--;
            break;
        default:
            break;
    }
    if (stuck) {
        Object *obj = this->getMap()->getFromCell(pos);
        if (obj != nullptr && obj->getType() == ObjTypes::Friendly) {
            this->attack(obj);
        }
    }
}

void Enemy::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::saveToJson(savefile);
}

void Enemy::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::loadFromJson(savefile);
}
