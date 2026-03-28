#ifndef GAME_STRONGERENEMY_H
#define GAME_STRONGERENEMY_H

#include "../basics/Entity.h"
#include "../Weapons/Knife.h"

class StrongerEnemy : public Entity {
public:
    StrongerEnemy() : Entity("StrongerEnemy", 4, ObjTypes::Hostile, 1, {new Knife(this)}) {
    };

    void update() override;
    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif