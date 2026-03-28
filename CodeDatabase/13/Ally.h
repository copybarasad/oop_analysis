#ifndef GAME_ALLY_H
#define GAME_ALLY_H

#include "../basics/Entity.h"
#include "../basics/Weapon.h"

class Ally : public Entity {
public:
    Ally() : Entity("Ally", 7, ObjTypes::Friendly, 1, {new Weapon(this)}) {
    };

    void update() override;

    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif
