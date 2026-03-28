#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <nlohmann/json.hpp>
#include  <iostream>
#include "Coordinates.h"
#include "ObjTypes.h"

class Map;
class GameLogic;

class Object {
private:
    size_t id_ = 0;
    Map *map_ = nullptr;
    GameLogic *glogic_ = nullptr;
    std::string name_;
    ObjTypes type_;
    bool ignorWhenRaycasting_;
    Coordinates position_;

public:
    explicit Object(
        const std::string &name = "Object",
        const ObjTypes type = ObjTypes::Neither,
        const bool ignorWhenRaycasting = false) : name_(name), type_(type),
                                            ignorWhenRaycasting_(ignorWhenRaycasting) {
    };

    virtual ~Object();

    std::string getName();

    size_t getId() const;

    GameLogic *getGameLogic();

    Map *getMap();

    ObjTypes getType() const;

    void setId(size_t new_id);

    void setGameLogic(GameLogic *glogic);

    Coordinates getPosition() const;

    void setPosition(const Coordinates &pos);

    virtual void update() {
    };

    virtual void saveToJson(nlohmann::json *savefile);

    virtual void loadFromJson(nlohmann::json *savefile){};
};

#endif
