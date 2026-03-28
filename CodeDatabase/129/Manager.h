#pragma once
#include <vector>
#include <map>
#include <string>
#include "../utils/Point.h"
#include "../utils/Functions2.h"
#include "../utils/Values.h"
#include "../entities/Player.h"
#include "../entities/Enemy.h"
#include "../spells/Hand.h"
#include "../entities/EnemyTower.h"
#include "../../ui/GField.h"
#include "../../saving/Saving.h"
#include "../../command_manager/Control.h"
#include "Field.h"

class Manager{
private:
    int current_level;
    int moves;
    std::map<char, int> killed;

    Field field;
    Player player;
    std::vector<Enemy> enemies;
    Hand hand;
    EnemyTower enemy_tower;
public:
    Manager();
    Manager(int height, int width, int hand_size, int level);
    Manager(int height, int width, std::vector<std::string> spells, int level);
    ~Manager();

    //для логической работы игры
    bool process(char c, int level);
    bool use_spell(char spell_c, char direct_c);
    bool check_condition(int level);
    std::vector<std::string> get_reset_spells();

    //для сохранения
    Saving serialize() const;
    void deserialize(const Saving& data);

    //для отрисовки - в Manager_get.cpp
    GField sync();
    int get_moves();
    int get_level();
    std::map<char, int> get_killed();
    int get_player_lifes();
    bool get_hand_can_use_spell(char c);
    int get_hand_max_count();
    std::vector<std::string> hand_get_my_spells();
    bool get_player_is_close_combat();
    bool get_enemy_tower_time_to_kill();

    //отдельно сохраняем врагов - так как они хранятся массивом
    std::vector<Saving::EnemyData> enemies_serialize()const;
    void enemies_deserialize(const std::vector<Saving::EnemyData>& data);

private:
    //внутренняя логика игры - в Manager_in.cpp
    void move_enemies();
    void check_enemies();
    bool hurt_enemies(Point dest, int damage);
    void player_hit_enemy(char c);
    bool move_player(char c);
    bool enemy_tower_hit_player();
};
