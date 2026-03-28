#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

namespace GameConstants {
	constexpr int FIELD_WIDTH = 15;
	constexpr int FIELD_HEIGHT = 15;
	
	constexpr int INITIAL_GAME_LEVEL = 1;
	
	constexpr int POINTS_FOR_ENEMY_KILL = 10;
	constexpr int POINTS_FOR_BUILDING_KILL = 20;
	constexpr int POINTS_FOR_ALLY_KILL = 10;
	
	constexpr int SPELL_COST = 10;
	
	constexpr int ENEMY_ATTACK = 5;
	constexpr int ENEMY_HP = 20;
	
	constexpr int TOWER_ATTACK_RANGE = 3;
	
	constexpr int IMPASSABLE_CELL_PROBABILITY = 15;
	constexpr int SLOWING_CELL_PROBABILITY = 10;
	constexpr int RANDOM_RANGE_MAX = 99;
	
	namespace Directions {
		constexpr int ADJACENT_8_COUNT = 8;
		constexpr int DX[ADJACENT_8_COUNT] = {-1, 1, 0, 0, -1, -1, 1, 1};
		constexpr int DY[ADJACENT_8_COUNT] = {0, 0, -1, 1, -1, 1, -1, 1};
	}
}

#endif
