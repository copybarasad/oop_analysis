#ifndef TURNS_HPP_INCLUDED
#define TURNS_HPP_INCLUDED

#include <vector>

#include "position.hpp"
#include "world.hpp"

bool melee_attack(World& w, std::vector<Position>& to_redraw);
bool ranged_attack(World& w, std::vector<Position>& to_redraw);

void enemies_turn(World& w, std::vector<Position>& to_redraw);
void spawners_turn(World& w, std::vector<Position>& to_redraw);
void towers_turn(World& w, std::vector<Position>& to_redraw);
void allies_turn(World& w, std::vector<Position>& to_redraw);

#endif

