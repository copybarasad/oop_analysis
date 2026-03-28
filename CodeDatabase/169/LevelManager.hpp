#pragma once
#include "Field.hpp"
#include "WorldManager.hpp"
#include "Enemy.hpp"
#include "EnemyBuilding.hpp"
#include "EnemyTower.hpp"
#include "Coords.hpp"

class LevelManager {
public:
	static constexpr int FIELD_WIDTH = 12;
	static constexpr int FIELD_HEIGHT = 12;
	
	static void initializeLevel(int level, Field& field, WorldManager& world);
	static int calculateKillsRequired(int level);
};









