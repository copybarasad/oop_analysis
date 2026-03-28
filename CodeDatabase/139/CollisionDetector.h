#pragma once
#include "DataTypes/Point.h"
#include <vector>


class ExecuteUseCase;

class CollisionDetector
{
public:
	bool checkMapCollision(const ExecuteUseCase& execute, const Point& point, const Point& vector) const;
	bool checkEntitiesCollision(const Point& entityFirst, const Point& entitySecond, const Point& vector) const;
	bool checkSlowerCellsCollision(const ExecuteUseCase& execute) const;
	bool checkTrapsCollision(const ExecuteUseCase& execute, const Point& vector) const;
};