#ifndef GAMELB1_ENEMYTOWER_H
#define GAMELB1_ENEMYTOWER_H

#include "Entity.h"

class Game;

class EnemyTower : public Entity {
public:
    EnemyTower(int health, int damage, int radius, int x, int y, int cooldown);

    void update(Game& game);

    void takeDamage(int damage);
    bool isDestroyed() const;
    int getHealth() const;

private:
    int health_;
    int damage_;
    int radius_;
    int initial_cooldown_;
    int current_cooldown_;
};

#endif //GAMELB1_ENEMYTOWER_H