#pragma once
#include <vector>
#include <stdexcept>
#include "cell.h"
#include "position.h"
#include "trap.h"

class GameField {
public:
	GameField(int width, int height);

	bool IsWithinBounds(const Position& pos) const; 
	bool IsWalkable(const Position& pos) const;
	void SetCell(const Position& pos, CellType type);
	CellType GetCellType(const Position& pos) const;
	int GetWidth() const { return width_; }
	int GetHeight() const { return height_; }

	bool PlaceTrap(const Position& pos, int damage = 30);
    bool HasTrap(const Position& pos) const; 
    int TriggerTrap(const Position& pos);
    void RemoveTrap(const Position& pos);

	bool IsValidPosition(const Position& pos) const;
	int GetTrapDamage(const Position& pos) const;

private:
	int width_;
	int height_;
	std::vector<std::vector<Cell>> cells_;
	std::vector<std::vector<Trap>> traps_;

	void GenerateObstacles();

	friend class EnemyManager;
	friend class UIManager;
	friend class Pathfinder;
};
