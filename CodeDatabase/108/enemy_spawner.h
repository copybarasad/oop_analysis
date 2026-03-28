#ifndef OOP_ENEMY_SPAWNER_H
#define OOP_ENEMY_SPAWNER_H

#include "position.h"
#include "enemy.h"


class EnemySpawner : public Entity {

    int spawn_interval;
    int current_cooldown;      // уникальные only
    int cnt_spawned;

public:
    explicit EnemySpawner(std::string name, const Position& position, int health = 50, int spawn_interval = 6, int cnt_spawned = 0);

    void decrease_cooldown();
    bool should_spawn() const;
    Enemy spawn_enemy();
    int get_spawn_cooldown() const;
    int get_max_cooldown() const;


};

#endif //OOP_ENEMY_SPAWNER_H