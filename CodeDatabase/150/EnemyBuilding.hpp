#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

#include <stdexcept>
#include <iostream>
#include "Building.hpp"

class EnemyBuilding : public Building{
private:
    int cooldown_;
    int spawnCounter_;

public:
    EnemyBuilding(int x, int y, int health = 50, int cooldown = 10);
    
    void update() override;
    bool isActive() const override;
    
    int getCooldown() const;
    bool canSpawn() const;
    void resetCounter();

    void serialize(std::ostream& os) const override {
        Building::serialize(os);
        os.write(reinterpret_cast<const char*>(&cooldown_), sizeof(cooldown_));
        os.write(reinterpret_cast<const char*>(&spawnCounter_), sizeof(spawnCounter_));
    }
    
    void deserialize(std::istream& is) override {
        Building::deserialize(is);
        is.read(reinterpret_cast<char*>(&cooldown_), sizeof(cooldown_));
        is.read(reinterpret_cast<char*>(&spawnCounter_), sizeof(spawnCounter_));
    }
};

#endif