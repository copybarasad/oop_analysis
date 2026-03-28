#ifndef POSITION_H
#define POSITION_H

struct Position {
	int x;
	int y;

	Position(int posX = 0, int posY = 0) : x(posX), y(posY) {}
};

#endif

