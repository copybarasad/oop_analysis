#include "Object.h"
#include "../Entities/Player.h"
#include "Map.h"
#include "GameLogic.h"

int GameLogic::addObject(Object *obj, Coordinates pos) {
    if (obj != nullptr && !this->map_.isOccupied(pos)) {
        this->map_.addToCell(pos, obj);
        obj->setGameLogic(this);
        for (size_t i = 0; i < this->updateble_.size(); i++) {
            if (this->updateble_[i] == nullptr) {
                obj->setId(i);
                this->updateble_[i] = obj;
                return 0;
            }
        }
        obj->setId(this->updateble_.size());
        this->updateble_.push_back(obj);
        Player *playerPtr = dynamic_cast<Player *>(obj);
        if (playerPtr) {
            this->player_ = playerPtr;
        }
        return 0;
    }
    return 1;
}


int GameLogic::removeObject(const size_t id) {
    if (id < this->updateble_.size() && this->updateble_[id] != nullptr) {
        this->map_.removeFromCell(this->updateble_[id]->getPosition());
        this->updateble_[id] = nullptr;
        return 0;
    }
    return 1;
}

void GameLogic::update() const {
    for (Object *i: this->updateble_) {
        if (i != nullptr) {
            i->update();
        }
    }
}

Map &GameLogic::getMap() {
    return this->map_;
}

void GameLogic::addScore(const int points) {
    this->score_ += points;
    if (this->score_ % 3 == 0) {
        this->getPlayer()->getSpellBook()->addRandomSpell();
    }
}

int GameLogic::getScore() const {
    return this->score_;
}

Player *GameLogic::getPlayer() const {
    return this->player_;
}

void GameLogic::clear() {
    if (!this->updateble_.empty()) {
        for (auto i: this->updateble_) {
            if (i != nullptr) {
                this->removeObject(i->getId());
            }
        }
        this->updateble_.clear();
    }
}

GameLogic::~GameLogic() {
    this->clear();
    delete this->player_;
}

void GameLogic::newMap(const int width, const int height) {
    this->map_ = Map(width, height, 3);
}

void GameLogic::saveJson(nlohmann::json *savefile) const {
    if (!savefile) return;

    nlohmann::json &objects = (*savefile)["objects"];
    objects = nlohmann::json::array();

    for (auto obj: this->updateble_) {
        if (obj != nullptr) {
            nlohmann::json obj_json;
            obj->saveToJson(&obj_json);
            objects.push_back(obj_json);
        }
    }
}

void GameLogic::setScore(const int score) {
    this->score_ = score;
}
