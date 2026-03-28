#include <iostream>
#include <vector>
#include <stdexcept>
#include <format>

#ifndef CELL_H
#define CELL_H

class Cell
{
private:
	bool passable;
	bool trapped;
	int damage;
public:
	Cell();
	Cell(bool passable, bool trapped, int damage);

	bool getPassable() const;
	bool getTrapped() const;
	int getDamage() const;

	void setPassable(bool passableValue);
	void setTrapped(bool trappedValue);
	void setDamage(int damageValue);
};

#endif // CELL_H