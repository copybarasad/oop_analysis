#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include "hero.hpp"
#include "fieldcell.hpp"
#include "enemy.hpp"
#include "tower.hpp"
#include "ally.hpp"
#include "attacktower.hpp" 

template<typename T>
using EntityWithPos = std::pair<T, std::pair<int, int>>;

class EntityManager {
public:
    int enemy_id_counter = 1000;
    int level = 1;
    int ally_id_counter = 2000;

    std::vector<EntityWithPos<Enemy>> enemies;
    std::vector<EntityWithPos<Tower>> towers;
    std::vector<EntityWithPos<Ally>> allies;
    std::vector<EntityWithPos<AttackTower>> attack_towers;

    EntityManager(int count_enemy, int level, int width = 0, int height = 0);
    void create_ally(int id, int x, int y);
    void create_mob(int level, int id, int x, int y);
    void create_tower(int cooldown, int level, int id);
    void create_attack_tower(int id, int x, int y);
    bool canMoveTo(const std::vector<std::vector<FieldCell>>& box, int x, int y) const;
    void move_attack_towers(Field& field, EntityManager& roster, Hero& player);
    void move_ally(std::vector<std::vector<FieldCell>>& box, int hero_x, int hero_y);
    void move_mob(std::vector<std::vector<FieldCell>>& box, Hero& player, int hero_x, int hero_y);
    void move_tower(std::vector<std::vector<FieldCell>>& box);
    void atack_enemy(int dmg, int id);
    void update();

    const std::vector<EntityWithPos<Enemy>>& get_enemies() const { return enemies; }
    const std::vector<EntityWithPos<Tower>>& get_towers() const { return towers; }
    const std::vector<EntityWithPos<Ally>>& get_allies() const { return allies; }
    const std::vector<EntityWithPos<AttackTower>>& get_attack_towers() const { return attack_towers; }
    std::vector<EntityWithPos<Enemy>>& get_enemies() { return enemies; }
    std::vector<EntityWithPos<Tower>>& get_towers() { return towers; }
    std::vector<EntityWithPos<Ally>>& get_allies() { return allies; }
    std::vector<EntityWithPos<AttackTower>>& get_attack_towers() { return attack_towers; }
};