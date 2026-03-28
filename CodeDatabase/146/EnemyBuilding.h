#pragma once
#include "Building.h"
#include "Enemy.h"


class EnemyBuilding : public Building {
private:
	int count_step_spawn;
public:
	EnemyBuilding(double h, Coords crd, int c) : Building(h, crd), count_step_spawn(c){}
	void do_turn(GameArea& gamearea);
	void spawn_enemy(GameArea& gamearea);
	Coords find_nearest_free_cell(GameArea& gamearea);
};

