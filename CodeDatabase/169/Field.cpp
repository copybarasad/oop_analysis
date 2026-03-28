#include "Field.hpp"
#include <stdexcept>
#include <algorithm>
#include <vector>

Field::Field(int width, int height)
	: width(width), height(height), cells() {
	if (width < 10 || width > 25 || height < 10 || height > 25) {
		throw std::invalid_argument("Field size must be between 10 and 25 for both dimensions.");
	}
	cells.resize(width);
	for (int x = 0; x < width; ++x) {
		cells[x].resize(height);
	}
	initImpassable();
}

Field::Field(const Field& other)
	: width(other.width),
	  height(other.height),
	  cells(other.cells) {}

Field::Field(Field&& other) noexcept
	: width(other.width),
	  height(other.height),
	  cells(std::move(other.cells)) {
	other.width = 0;
	other.height = 0;
}

Field& Field::operator=(const Field& other) {
	if (this != &other) {
		width = other.width;
		height = other.height;
		cells = other.cells;
	}
	return *this;
}

Field& Field::operator=(Field&& other) noexcept {
	if (this != &other) {
		width = other.width;
		height = other.height;
		cells = std::move(other.cells);
		other.width = 0;
		other.height = 0;
	}
	return *this;
}

int Field::getWidth() const noexcept { return width; }
int Field::getHeight() const noexcept { return height; }

bool Field::inBounds(const Coords& p) const noexcept {
	return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
}

Cell& Field::cellAt(const Coords& p) {
	return cells.at(p.x).at(p.y);
}

const Cell& Field::cellAt(const Coords& p) const {
	return cells.at(p.x).at(p.y);
}

void Field::initImpassable() {
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			if ((x == 0 && y == height - 1) || (x == width - 1 && y == 0)) {
				cells[x][y] = Cell(true, CellType::Normal);
			} else if (x % 3 == 0 && y % 2 == 0) {
				cells[x][y] = Cell(false, CellType::Impassable);
			} else if (x % 2 == 0 && y % 3 == 0) {
				cells[x][y] = Cell(true, CellType::Slowing);
			} else {
				cells[x][y] = Cell(true, CellType::Normal);
			}
		}
	}
}

void Field::print(const Player& player, int score, const std::vector<Coords>* traps) const {
	std::cout << "|| Управление: w(вверх), s(вниз), a(влево), d(вправо), f(атака), t(сменить режим), q(выход)\n";
	std::cout << "|| Размер поля (" << width << " x " << height << ")\n";
	std::cout << "|| P - Игрок, E - Враг, B - Вражеское здание, T - Вражеская башня, t - Ловушка\n|| # - Непроходимая клетка, ~ - Замедляющая клетка(игрок пропускает ход)\n";
	std::cout << "-----------------------------\n";


	std::cout << "   ";
	for (int x = 0; x < width; ++x) {
		if (x < 10) {
			std::cout << x << ' ';
		} else {
			std::cout << x;
		}
	}
	std::cout << "  X\n";

	for (int y = 0; y < height; ++y) {
		if (y < 10) {
			std::cout << ' ' << y << ' ';
		} else {
			std::cout << y << ' ';
		}
		
		for (int x = 0; x < width; ++x) {
			const Cell& c = cells[x][y];
			Coords pos{x, y};
			bool hasTrap = false;
			if (traps) {
				for (const auto& trapPos : *traps) {
					if (trapPos == pos) {
						hasTrap = true;
						break;
					}
				}
			}
			
			if (!c.isWalkable()) {
				std::cout << '#';
			} else if (c.getOccupant() == OccupantType::Player) {
				std::cout << 'P';
			} else if (c.getOccupant() == OccupantType::Enemy) {
				std::cout << 'E';
			} else if (c.getOccupant() == OccupantType::Building) {
				std::cout << 'B';
			} else if (c.getOccupant() == OccupantType::Tower) {
				std::cout << 'T';
			} else if (hasTrap) {
				std::cout << 't';
			} else {
				switch (c.getType()) {
					case CellType::Slowing:
						std::cout << '~';
						break;
					case CellType::Normal:
					default:
						std::cout << '.';
						break;
				}
			}
			std::cout << ' ';
		}
		std::cout << '\n';
	}
	std::cout << " Y\n";
	std::cout << "-----------------------------\n";
	std::cout << "|| Здоровье: " << player.getHP() << "/" << player.getMaxHP()
			  << "\n|| Режим атаки: " << (player.getMode() == AttackMode::Melee ? "Ближний бой" : "Дальний бой")
			  << "\n|| Очки (убийств): " << score
			  << "\n";
}

