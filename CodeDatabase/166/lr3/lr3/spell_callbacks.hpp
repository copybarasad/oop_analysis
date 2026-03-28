#ifndef SPELL_CALLBACKS_HPP_INCLUDED
#define SPELL_CALLBACKS_HPP_INCLUDED

#include <tuple>
#include <vector>

#include "position.hpp"
#include "world.hpp"

void summon_register_created(const std::vector<Position>& ps, int hp, int dmg);
void trap_register_created(const Position& pos, int dmg);
void tower_register_created(const Position& pos, int dmg, int rad);
void flush_created_entities(World& w);

#endif

