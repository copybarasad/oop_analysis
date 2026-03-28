#pragma once
struct Coords {
	int x;
	int y;
	Coords(int x, int y) : x(x), y(y) {}
	Coords operator+(Coords& other) {
		return Coords(x + other.x, y + other.y);
	}
	bool operator!=(Coords& other) {
		return x != other.x || y != other.y;
	}
};