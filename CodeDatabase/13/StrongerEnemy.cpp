#include "StrongerEnemy.h"

#include "Player.h"
#include "../basics/GameLogic.h"

void StrongerEnemy::update() {
    Coordinates pos = this->getPosition();
    int range = this->getWeapon()->range_;

    for (int y = -range; y < range + 1; y++) {
        for (int x = -range; x < range + 1; x++) {
            pos = this->getPosition();
            pos.x_ += x;
            pos.y_ += y;
            Object *obj = this->getMap()->getFromCell(pos);
            if (obj != nullptr && obj->getType() == ObjTypes::Friendly) {
                this->attack(obj);
            }
        }
    }

    pos = this->getPosition();
    int pl_x = this->getGameLogic()->getPlayer()->getPosition().x_;
    int pl_y = this->getGameLogic()->getPlayer()->getPosition().y_;

    if (pos.x_ > pl_x) {
        this->move(-1, 0);
        pos.x_--;
    } else if (pos.x_ < pl_x) {
        this->move(1, 0);
        pos.x_++;
    }

    if (pos.y_ > pl_y) {
        this->move(0, -1);
        pos.y_--;
    } else if (pos.y_ < pl_y) {
        this->move(0, 1);
        pos.y_++;
    }
}

void StrongerEnemy::saveToJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::saveToJson(savefile);
}

void StrongerEnemy::loadFromJson(nlohmann::json *savefile) {
    if (!savefile) return;
    Entity::loadFromJson(savefile);
}
