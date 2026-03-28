#ifndef ENEMY_H
#define ENEMY_H

#include "game_objects.h"
#include <nlohmann/json.hpp>

class Enemy : public GameObject {
private:
    int damage;
    int Strong;
    
public:
    Enemy(int X, int Y, int hp, int dam, int strong);
    int getDamage();

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);

};

#endif