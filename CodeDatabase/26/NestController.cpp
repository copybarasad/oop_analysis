#include "NestController.hpp"
#include "Field.hpp"
#include "Enemy.hpp"
#include <iostream>

bool NestController::trySpawnEnemyAdjacent(Field& f, Field::NestEntry& n) {
	static const Position deltas[4] = { {0,-1}, {0,1}, {-1,0}, {1,0} };
	for (const auto& d : deltas) {
		Position p{ n.pos.x + d.x, n.pos.y + d.y };
		if (f.isInside(p) && f.cellIsFreeAndPassable(p)) {
			f.enemies_.push_back(Field::EnemyEntry{ Enemy(f.enemySpawnHp_, f.enemySpawnDmg_), p });
			extern bool g_verbose;
			if (g_verbose) {
				std::cout << "[DBG] Nest at (" << n.pos.x << "," << n.pos.y << ")"
				          << " spawns enemy at (" << p.x << "," << p.y << ")"
				          << " hp=" << f.enemySpawnHp_
				          << " dmg=" << f.enemySpawnDmg_ << std::endl;
			}
			return true;
		}
	}
	return false;
}

void NestController::processNests(Field& field) {
	for (auto& n : field.nests_) {
		n.counter -= 1;
		if (n.counter <= 0) {
			(void)trySpawnEnemyAdjacent(field, n);
			n.counter = n.nest.spawnPeriod();
		}
	}
}
