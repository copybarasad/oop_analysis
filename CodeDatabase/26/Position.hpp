#pragma once

struct Position {
	int x{0};
	int y{0};
	bool operator==(const Position& other) const noexcept {
		return x == other.x && y == other.y;
	}
	bool operator!=(const Position& other) const noexcept {
		return !(*this == other);
	}
};
