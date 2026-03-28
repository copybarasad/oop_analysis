#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "Entity.h"

class EnemyBuilding : public Entity {
public:
    EnemyBuilding(int x, int y, int health, int spawn_rate);
    void update(Game& game) override;
    
    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
    bool shouldSpawn();
    void tick();
    
    int spawn_rate_;
    int turns_until_spawn_;
};

#endif