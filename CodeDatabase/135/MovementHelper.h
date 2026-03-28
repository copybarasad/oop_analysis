#ifndef MOVEMENT_HELPER_H
#define MOVEMENT_HELPER_H

class GameField;
class EntityManager;

class MovementHelper {
private:
	GameField& field;
	EntityManager& entity_manager;

public:
	MovementHelper(GameField& game_field, EntityManager& em);
	
	bool moveEntityTowards(int entity_index, int target_index);
	int getManhattanDistance(int index1, int index2) const;
	bool isAdjacent(int index1, int index2) const;
};

#endif
