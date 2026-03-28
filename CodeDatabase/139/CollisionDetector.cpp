#include "CollisionDetector.h"
#include "ExecuteUseCase.h"
#include <iostream>


bool CollisionDetector::checkMapCollision(const ExecuteUseCase& execute, const Point& point, const Point& vector) const
{
	Point entityPoint = point + vector;

	int width = execute.getGameBoard()->getField()->getWidth();
	int heigth = execute.getGameBoard()->getField()->getHeight();

	if (entityPoint.X() >= width || entityPoint.X() < 0)
		return true;

	if (entityPoint.Y() >= heigth || entityPoint.Y() < 0)
		return true;


	entityPoint = point;
	int normalizedValue;
	if (vector.X() != 0 || vector.Y() != 0)
	{
		int direction = vector.X() != 0 ? vector.X() : vector.Y();
		normalizedValue = direction / abs(direction);
		Point newVector(vector.X() != 0 ? normalizedValue : 0, vector.X() != 0 ? 0 : normalizedValue);
		for (int i = 0; i < abs(direction); ++i)
		{
			entityPoint += newVector;
			if (!execute.getGameBoard()->getField()->getCell(entityPoint).isObstruction())
				return true;
		}
	}

	return false;
}


bool CollisionDetector::checkEntitiesCollision(const Point& entityFirst, const Point& entitySecond, const Point& vector) const
{
	if (entitySecond + vector == entityFirst)
		return true;
	return false;
}


bool CollisionDetector::checkSlowerCellsCollision(const ExecuteUseCase& execute) const
{
	auto& playerPos = execute.getGameBoard()->getPlayer()->getPosition();
	auto& fieldCell = execute.getGameBoard()->getField()->getCell(playerPos);
	if (fieldCell.isSlow())
		return true;

	return false;
}


bool CollisionDetector::checkTrapsCollision(const ExecuteUseCase& execute, const Point& vector) const
{
	for (auto& trap : execute.getGameBoard()->getTraps())
	{
		if (trap.getPosition() == vector)
			return true;
	}

	return false;
}