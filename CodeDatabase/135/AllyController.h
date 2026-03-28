#ifndef ALLY_CONTROLLER_H
#define ALLY_CONTROLLER_H

#include "GameConstants.h"
#include <vector>

class GameField;
class EntityManager;
class MovementHelper;
class RandomGenerator;

class AllyController {
private:
	GameField& field;
	EntityManager& entity_manager;
	MovementHelper& movement_helper;
	RandomGenerator& rng;
	
	void moveAllyTowardsPlayer(int ally_index, int player_index);
	void attackNearbyEnemies(int ally_index, int player_index);

public:
	AllyController(GameField& game_field, EntityManager& em, MovementHelper& mh, 
	               RandomGenerator& random_gen);
	
	void processAllyTurn(int player_index);
};

#endif
