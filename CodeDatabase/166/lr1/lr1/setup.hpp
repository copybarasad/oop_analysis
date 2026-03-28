#ifndef SETUP_HPP
#define SETUP_HPP

#include <vector>
#include <string>
#include "position.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "spawner.hpp"

// параметры нового мира
struct GameParams {
    int width{12};
    int height{12};

    Position heroStart{0,0};
    int heroHp{30};
    int heroDmg{6};

    int spawnPeriod{3};
    std::vector<Position> spawners;

    std::vector<Position> walls;
    std::vector<Position> slows;
};

// запрос всех параметров
GameParams prompt_game_params();

// создаём мир по параметрам
bool build_world_from_params(const GameParams& p,
                             Field& outField,
                             Hero& outHero,
                             std::vector<Spawner>& outSpawners);

#endif