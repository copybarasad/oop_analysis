#include "MovementHelper.h"
#include "GameField.h"
#include "EntityManager.h"
#include <cmath>

MovementHelper::MovementHelper(GameField& game_field, EntityManager& em)
	: field(game_field), entity_manager(em) {}

int MovementHelper::getManhattanDistance(int index1, int index2) const {
	int x1, y1, x2, y2;
	field.indexToCoord(index1, x1, y1);
	field.indexToCoord(index2, x2, y2);
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool MovementHelper::isAdjacent(int index1, int index2) const {
	return getManhattanDistance(index1, index2) == 1;
}

bool MovementHelper::moveEntityTowards(int entity_index, int target_index) {
	int entity_x, entity_y;
	field.indexToCoord(entity_index, entity_x, entity_y);
	
	int target_x, target_y;
	field.indexToCoord(target_index, target_x, target_y);
	
	int dx = 0, dy = 0;
	if (target_x > entity_x) dx = 1;
	else if (target_x < entity_x) dx = -1;
	
	if (target_y > entity_y) dy = 1;
	else if (target_y < entity_y) dy = -1;
	
	int new_x = entity_x + dx;
	int new_y = entity_y + dy;
	
	if (!field.isValidPosition(new_x, new_y)) {
		return false;
	}
	
	int new_index = field.coordToIndex(new_x, new_y);
	
	if (field.getCell(new_index).isPassable() && !entity_manager.hasEntity(new_index)) {
		entity_manager.moveEntity(entity_index, new_index);
		return true;
	}
	
	return false;
}
