#ifndef BUILDING_H
#define BUILDING_H

#include "game_objects.h"
#include <nlohmann/json.hpp>

class Building : public GameObject {
private:
    int cooldown_spawn;
    int spawn_timer;
    
public:
    Building(int X, int Y, int hp, int cooldown, int spawn);
    int getDamage();
    int getCooldown();
    int getSpawn();
    void setSpawnTimer(int a);

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
};

#endif