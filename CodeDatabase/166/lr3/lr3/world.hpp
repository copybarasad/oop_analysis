#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include <string>
#include <vector>

#include "field.hpp"
#include "hero.hpp"
#include "enemy.hpp"
#include "spawner.hpp"
#include "traps.hpp"
#include "towers.hpp"
#include "ally.hpp"
#include "hand.hpp"
#include "spell.hpp"

struct World {
    Field field;
    Hero hero;
    std::vector<Enemy> enemies;
    std::vector<Spawner> spawners;
    std::vector<Trap> traps;
    std::vector<Tower> towers;
    std::vector<Ally> allies;

    int kills{0};
    int awardsGiven{0};
    Hand hand;
    BoostState boosts;

    int level{1};
    int baseHeroHp{0};

    World(Field f, Hero h, std::vector<Enemy> es, std::vector<Spawner> ss, std::size_t handCap);
};

void save_world_to(const World& w, const std::string& path);
World load_world_from(const std::string& path);

int alive_enemies(const World& w);
void maybe_award_card(World& w);

int max_enemy_hp_for_level(int level);
int max_spawner_hp_for_level(int level);

bool is_enemy_at(const World& w, const Position& p, int* idx = nullptr);
bool is_spawner_at(const World& w, const Position& p);
int  spawner_index_at(const World& w, Position p);
int  trap_index_at(const World& w, const Position& p);
int  tower_index_at(const World& w, const Position& p);
bool is_ally_at(const World& w, const Position& p);

bool cell_free_for_enemy(const World& w, const Position& p);
bool cell_free_for_hero(const World& w, const Position& p);
bool cell_free_for_ally(const World& w, const Position& p);

void remove_half_hand_random(Hand& hand);
void proceed_to_next_level(World& world);

#endif

