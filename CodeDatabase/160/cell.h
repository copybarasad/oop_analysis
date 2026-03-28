#ifndef INCLUDE_CELL
#define INCLUDE_CELL

#include "position.h"

enum class CellType { 
	Empty, 
	Wall, 
	Player, 
	Enemy, 
	EnemyTower, 
	Freeze, 
	Trap,
    FriendEnemy
};

class Cell {
private:
	CellType type_;

public:
	Cell() : type_(CellType::Empty) {}
    explicit Cell(CellType type) : type_(type) {}

	CellType type() const { return this->type_; }

	void set_type(CellType ct) { this->type_ = ct; }
};

#endif
