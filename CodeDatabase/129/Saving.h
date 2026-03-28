#pragma once
#include <vector>
#include <map>
#include <string>
//структура для хранения всем нам нужных данных
//мы не можем использовать сами объекты, так как их будет
//сложно представить в char* виде целиком - поэтому мы сразу сохраняем
//ВСЕ поля ВСЕХ полей

class Saving{
public:
    class PlayerData{
    public:
        int lifes;
        int damage;
        int x;
        int y;
        bool close_combat;
    };
    
    class HandData{
    public:
        std::vector<std::string> acceptable;
        std::vector<std::string> spells;
        int last_spell;
        int max_count;
    };

    class EnemyData{
    public:
        int x;
        int y;
        int lifes;
        int damage;
    };

    class CellData{
    public:
        bool impassible;
        bool is_there_player;
        bool is_there_enemy;
        bool trap;
        bool enemy_tower;
    };

    class FieldData{
    public:
        int height;
        int width;
        std::vector<std::vector<CellData>> cells;
    };

    class ManagerData{
    public:
        int level;
        FieldData field;
        PlayerData player;
        std::vector<EnemyData> enemies;
        std::map<char, int> killed;
        HandData hand;
    };

    ManagerData manager;
};
