#ifndef BUILDING_H
#define BUILDING_H

#include "entity.h"

class Building : public Entity {
public:
    Building(int id, int health, int spawnInterval);
    EntityType getType() const override;
    int getDamage() const override;
    int getSpawnInterval() const;
    int getCounter() const;
    void tick();
    void resetCounter();
    bool shouldSpawn() const;
    Entity* produceEnemy(int newId);
    virtual void tryAttackIfTower(class Field&) {}
    virtual char getDisplaySymbol() const { return 'B'; }
    Building* asBuilding() override;
private:
    int spawnInterval_;
    int counter_;
};

#endif
