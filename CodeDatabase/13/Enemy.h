#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include "../basics/Entity.h"
#include "../basics/Weapon.h"


class Enemy : public Entity {
public:
    Enemy() : Entity("Enemy", 2, ObjTypes::Hostile, 1, {new Weapon(this)}) {
    };

    void update() override;


    void saveToJson(nlohmann::json *savefile) override;

    void loadFromJson(nlohmann::json *savefile) override;
};


#endif
