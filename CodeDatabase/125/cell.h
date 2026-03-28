#pragma once

enum class CellType { Empty, Player, Enemy, Obstacle };

class Cell {
public:
	Cell() : type_(CellType::Empty) {}
	CellType GetType() const { return type_; }
	void SetType(CellType type) { type_ = type; }

private:
	CellType type_;
};
