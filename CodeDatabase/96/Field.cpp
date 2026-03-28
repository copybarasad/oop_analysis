
#include <iostream>
#include <random>
#include <set>
#include <algorithm>
#include "Field.h"



Field::Field(int w, int h) : width(std::clamp(w,10,25)), height(std::clamp(h,10,25)), grid(height, std::vector<Cell>(width)) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> xDist(0, w - 1);
	std::uniform_int_distribution<> yDist(0, h - 1);


	auto getRandomFreePosition = [&](std::set<std::pair<int, int>>& used) -> std::pair<int,int> {
		std::pair<int,int> pos;
		do {
			pos = { xDist(gen), yDist(gen) };
		} while (used.count({pos.first, pos.second}) > 0);
		used.insert({pos.first, pos.second});
		return pos;
	};

	std::set<std::pair<int, int>> usedPositions;

	
	std::uniform_int_distribution<> wallCountDist(5, 10);
	int wallCount = wallCountDist(gen);
	for (int i = 0; i < wallCount; ++i) {
	std::pair<int,int> wallPos = getRandomFreePosition(usedPositions);
	grid[wallPos.second][wallPos.first].SetType(CellType::Wall);
	}

	std::uniform_int_distribution<> stunCountDist(3, 6);
	int stunCount = stunCountDist(gen);
	for (int i = 0; i < stunCount; ++i) {
	std::pair<int,int> stunPos = getRandomFreePosition(usedPositions);
	grid[stunPos.second][stunPos.first].SetType(CellType::Stun);
	}
}

Field::Field(dataField data): width(data.width), height(data.height), grid(data.grid){}

Field::Field(const Field& other)
    : width(other.width), height(other.height), grid(other.grid) {
}


Field::Field(Field&& other) noexcept
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}


Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}


Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}


Field::~Field() = default;



std::pair<int,int> Field::FindFreeAdjacentPosition(const std::pair<int,int>& pos) const {
	std::vector<std::pair<int,int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
	for (const auto& dir : directions) {
		std::pair<int,int> newPos = {pos.first + dir.first, pos.second + dir.second};
		if (newPos.first >= 0 && newPos.first < width && newPos.second >= 0 && newPos.second < height) {
			const Cell& cell = grid[newPos.second][newPos.first];
			if (cell.GetType() != CellType::Wall && !cell.IsOccupied()) {
				return newPos;
			}
		}
	}
	return pos;
}



int Field::Getheight() const noexcept{
	return height;
}

int Field::Getwidth() const noexcept{
	return width;
}

bool Field::inBounds(const std::pair<int,int>& p) const noexcept {
	return p.first >= 0 && p.second >= 0 && p.first < width && p.second < height;
}



void Field::SetCellType(const std::pair<int,int>& p, CellType t){
	if (!inBounds(p)) return;
	grid[p.second][p.first].SetType(t);
}


std::pair<int,int> Field::GetRandomFreePosition() const {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> xDist(0, width - 1);
	std::uniform_int_distribution<> yDist(0, height - 1);
	for (int attempts = 0; attempts < width * height * 2; ++attempts) {
		int x = xDist(gen);
		int y = yDist(gen);
		std::pair<int,int> p{x,y};
		if (inBounds(p) && GetCellTypeAt(p) != CellType::Wall && IsOccupiedAtPos(p) == false) return {x, y};
	}
	for (int y = 0; y < height; ++y) for (int x = 0; x < width; ++x) if (grid[y][x].GetType() != CellType::Wall) return {x,y};
	return {0,0};
}

bool Field::IsOccupiedAtPos(std::pair<int,int> p) const {
	if (!inBounds(p)) return false;
	return grid[p.second][p.first].IsOccupied();
}

void Field::ChangeisOccupiedAtpos(std::pair<int,int> p) {
	grid[p.second][p.first].SetOccupied(!grid[p.second][p.first].IsOccupied());
}

void Field::SetCellOccupied(const std::pair<int,int>& p, bool occ) {
	if (!inBounds(p)) return;
	grid[p.second][p.first].SetOccupied(occ);
}

CellType Field::GetCellTypeAt(const std::pair<int,int>& p) const {
	if (!inBounds(p)) return CellType::Empty;
	return grid[p.second][p.first].GetType();
}

int Field::GetTrapDamageAtPos(const std::pair<int,int>& p) const {
	if (!inBounds(p)) return 0;
	return grid[p.second][p.first].GetTrapDamage();
}

void Field::SetTrapAtPos(const std::pair<int,int>& p, int t) {
	if (!inBounds(p)) return;
	grid[p.second][p.first].SetTrap(t);
}

dataField Field::Save(){
	dataField data;
	data.width=width;
	data.height=height;
	data.grid=grid;
	return data;
}