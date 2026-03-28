#pragma once

struct Coords {
	int x;
	int y;

	constexpr Coords(int x = 0, int y = 0) noexcept 
		   : x(x), y(y) {}

	bool operator==(const Coords& other) const noexcept {
		return x == other.x && y == other.y;
	}
	bool operator!=(const Coords& other) const noexcept {
		return !(*this == other);
	}
};
