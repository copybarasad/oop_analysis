#include "MathUtils.h"
#include <cmath>

int MathUtils::CalculateDistance(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}



