#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Building.h"
#include "EnemyBuilding.h"
#include "Tower.h"
#include "GameArea.h"
#include <vector>
#include <memory>

struct GameState {
    int current_level;
    int area_width;
    int area_height;
    std::vector<std::vector<int>> cell_types;  // 0=BASIC, 1=BLOCKED, 2=SLOW

    double player_health;
    double player_damage;
    double player_experience;
    Coords player_coords;
    int player_hand_size;

    struct EnemyData {
        double health;
        double damage;
        Coords coords;

        EnemyData() : health(0), damage(0), coords(0, 0) {}

        EnemyData(double h, double d, Coords c) : health(h), damage(d), coords(c) {}
    };
    std::vector<EnemyData> enemies;

    struct SummonData {
        double health;
        double damage;
        Coords coords;

        SummonData() : health(0), damage(0), coords(0, 0) {}
        SummonData(double h, double d, Coords c) : health(h), damage(d), coords(c) {}
    };
    std::vector<SummonData> summons;

    struct BuildingData {
        std::string type;
        double health;
        Coords coords;
        int spawn_step;

        BuildingData() : type(""), health(0), coords(0, 0), spawn_step(0) {}

        BuildingData(std::string t, double h, Coords c, int s)
            : type(t), health(h), coords(c), spawn_step(s) {
        }
    };
    std::vector<BuildingData> buildings;

    GameState() : current_level(1), area_width(5), area_height(5),
        player_health(0), player_damage(0), player_experience(0),
        player_coords(0, 0), player_hand_size(3) {
    }
};
