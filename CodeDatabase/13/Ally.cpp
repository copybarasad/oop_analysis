#include "Ally.h"

#include <cmath>
#include <random>

#include "../basics/Map.h"

void Ally::update() {
    int stuck = 0;
    Coordinates pos = this->getPosition();
    int range = this->getWeapon()->range_;

    for (int y = -range; y < range + 1; y++) {
        for (int x = -range; x < range + 1; x++) {
            pos = this->getPosition();
            pos.x_ += x;
            pos.y_ += y;
            Object *obj = this->getMap()->getFromCell(pos);
            if (obj != nullptr && obj->getType() == ObjTypes::Hostile) {
                this->attack(obj);
                return;
            }
        }
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dir_dist(0, 3);
    int dir = dir_dist(gen);
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
        if (obj != nullptr && obj->getType() == ObjTypes::Hostile) {
            this->attack(obj);
        }
    }
}

void Ally::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::saveToJson(savefile);
}

void Ally::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::loadFromJson(savefile);
}
