#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "entity.hpp"
#include "defines.hpp"

class EnemyBuilding : public Entity {
private:
    int health;
    int turns_to_spawn;
    int initial_turns_to_spawn;

public:
    EnemyBuilding(int x, int y, int spawn_rate);

    bool shouldSpawn();
    void resetSpawnTimer();

    void takeDamage(int damage) override;
    void onDeath(Player* player) override;
    bool isAlive() const;
    int getHealth() const override;
    bool isEnemy() const override;

    EntityType getEntityType() const override { return EntityType::BUILDING; }

    virtual void removeFromBoard(GameBoard& board) override;

    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
};

#endif