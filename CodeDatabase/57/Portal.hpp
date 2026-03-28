#pragma once
#include "Enemy.hpp"
#include "Goblin.hpp"
#include "Slime.hpp"
#include "Demon.hpp"
#include "Shadow.hpp"
#include "./Field/Cell.hpp"
#include "./Field/Field.hpp"
#include "Entity.hpp"

class Portal: public Entity {

private:
    int RADIUS_SPAWN = 1;
    int spawn_frequency;

public: 
    Portal(int x, int y, int offsetX, int offsetY, int spawn_frequency = 5, int level = 1);

    ~Portal() {};

    Enemy* generate_emply(std::vector<std::vector<Cell*>>& vector_Cell, Field& field);
    int get_frequency() {return spawn_frequency; }
    void move_towards_target(Field&, InterfaceGameLogger&) override {}

};
