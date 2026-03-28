#pragma once

#include "field.hpp"
#include "entitymanager.hpp"

class Level {
public:
    int number;
    EntityManager roster;
    Field field;

    Level(int level_num, int enemy_count, int width, int height);
    Level(const Level&) = default;
    Level& operator=(const Level&) = default;
    Level(Level&&) = default; 
    Level& operator=(Level&&) = default;

    bool is_complete() const;
    void run_turn(Hero& player);
    void clear_dead_entities();
    void update_entities();
};