#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "entities/entity.hpp"
#include "field/field.hpp"
#include <vector>
#include <nlohmann/json_fwd.hpp>

class Level {
    Field &field;
    std::vector<Entity*> entities;
    int enemy_count;
    int level_number;

public:
    Level(Field &field, int level_number);
    ~Level();
    void add_entity(Entity *entity);
    void delete_entity(Entity *entity);
    std::vector<Entity*>& get_entities();
    int get_enemy_count() const;
    void init_entities(int width, int height);
    nlohmann::json save() const;
    
private:
    Point rand_pos(int width, int height);
};

#endif
