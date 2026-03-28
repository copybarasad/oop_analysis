#ifndef OOP_ENEMY_MANAGER_H
#define OOP_ENEMY_MANAGER_H

#include <vector>
#include "enemy_spawner.h"
#include "enemy_tower.h"


class GameState {
    std::vector<EnemySpawner> spawners;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;

    GameField& game_field;

    // Шаблонный метод для добавления сущностей во избежание дублирования кода (либо через лямбда выражения как в других случаях)
    template<typename T>
    void add_entity_impl(const T& entity, std::vector<T>& container, const std::string& error_msg);

public:
    explicit GameState(GameField& game_field_ref) :
        game_field(game_field_ref) {};

    void reset(); // для перехода на новый уровень

    // обработка спавнеров
    void add_spawner(const EnemySpawner& spawner);
    std::vector<EnemySpawner>& get_spawners();
    const std::vector<EnemySpawner>& get_spawners() const;


    // обработка врагов
    void add_enemy(const Enemy& enemy);
    std::vector<Enemy>& get_enemies();
    const std::vector<Enemy>& get_enemies() const;


    // обработка башен
    void add_tower(const EnemyTower& tower);
    std::vector<EnemyTower>& get_towers();
    const std::vector<EnemyTower>& get_towers() const;


    // поле
    const GameField& get_field() const;
    GameField& get_field();
    void set_new_field(const GameField& new_field) const;  // для перехода на новый уровень



    // МЕТОДЫ ДЛЯ ВСЕХ СУЩНОСТЕЙ (нужные здесь), для того, где не важно кто есть кто конкретно (просто сущность)
    void remove_dead_entities();
    Entity* find_nearest_target(const Position& from, int range);   // is used at Player::attack, DirectDamageSpell::cast

};

#endif