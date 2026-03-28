#include "cell.h"


Cell::Cell() : passable(true), trapped(false), damage(0){}
Cell::Cell(bool passable, bool trapped, int damage) : passable(passable), trapped(trapped), damage(damage){}

void Cell::setPassable(bool passableValue)
{
	passable = passableValue;
}

void Cell::setTrapped(bool trappedValue)
{
	trapped = trappedValue;
}

void Cell::setDamage(int damageValue)
{
	damage = damageValue;
}

bool Cell::getPassable() const
{
	return passable;
}

int Cell::getDamage() const
{
	return damage;
}

bool Cell::getTrapped() const
{
	return trapped;
}


std::ostream& operator << (std::ostream &os, const Cell &cell)
{
	os << cell.getPassable() << " " << cell.getTrapped() << " " << cell.getDamage();
	return os;
}

std::istream& operator >> (std::istream& in, Cell& cell)
{
	bool passable;
	bool trapped;
	int damage;
    in >> passable >> trapped >> damage;
    if (!in.good()) throw std::runtime_error("Failed to read cell parameters");
    if(damage < 0)
    {
    	throw std::invalid_argument("Wrong arguments for Cell!");
    }
    cell.setPassable(passable);
    cell.setTrapped(trapped);
    cell.setDamage(damage);
    return in;
}
