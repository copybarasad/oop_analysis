#include "PlayerController.h"
#include "FieldGrid.h"
#include <stdexcept>

PlayerController::PlayerController() : playerX(0), playerY(0) {
}

void PlayerController::SetPosition(int x, int y) {
	playerX = x;
	playerY = y;
}

int PlayerController::GetX() const {
	return playerX;
}

int PlayerController::GetY() const {
	return playerY;
}

bool PlayerController::Move(int deltaX, int deltaY, FieldGrid& grid) {
	int newX = playerX + deltaX;
	int newY = playerY + deltaY;
	
	if (!CanMoveTo(newX, newY, grid)) {
		return false;
	}
	
	playerX = newX;
	playerY = newY;
	return true;
}

bool PlayerController::CanMoveTo(int x, int y, FieldGrid& grid) const {
	return grid.IsValidPosition(x, y) && grid.IsCellPassable(x, y);
}



