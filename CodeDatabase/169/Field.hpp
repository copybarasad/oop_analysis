#pragma once
#include <vector>
#include <iostream>
#include "Cell.hpp"
#include "Coords.hpp"
#include "Player.hpp"

class Field {
public:
	Field(int width, int height);
	Field(const Field& other);
	Field(Field&& other) noexcept;
	Field& operator=(const Field& other);
	Field& operator=(Field&& other) noexcept;

	int getWidth() const noexcept;
	int getHeight() const noexcept;

	bool inBounds(const Coords& p) const noexcept;
	Cell& cellAt(const Coords& p);
	const Cell& cellAt(const Coords& p) const;

	void print(const Player& player, int score = 0, const std::vector<Coords>* traps = nullptr) const;

private:
	int width;
	int height;
	std::vector<std::vector<Cell>> cells;

	void initImpassable();
};

