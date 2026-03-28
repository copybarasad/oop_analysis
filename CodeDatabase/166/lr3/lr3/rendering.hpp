#ifndef RENDERING_HPP_INCLUDED
#define RENDERING_HPP_INCLUDED

#include "world.hpp"

inline constexpr int ORIGIN_Y = 1;
inline constexpr int ORIGIN_X = 2;

void draw_panel(const World& w, int start_col);
void draw_world_initial(const World& w);
void draw_cell(const World& w, const Position& p);
void redraw_all_cells(const World& w);

void draw_aim_cursor_single(const Position& p);
bool inBoundsSquare(const Field& f, const Position& tl, int side);
void draw_square_overlay(const World& w, const Position& tl, int side);

#endif

