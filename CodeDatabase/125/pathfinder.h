#pragma once
#include "game_field.h"
#include "position.h"
#include <vector>

class Enemy;

class Pathfinder {
public:
	static bool FindStep(const GameField& field, const Position& start, const Position& goal,
						 Position& step, const std::vector<Enemy>& others);
};
