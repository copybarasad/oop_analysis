#include "Point.h"


Point::Point()
	: x(0), y(0) { }


Point::Point(int x, int y)
	: x(x), y(y) { }


void Point::setX(int x)
{
	this->x = x;
}


void Point::setY(int y)
{
	this->y = y;
}


int Point::X() const
{
	return x;
}


int Point::Y() const
{
	return y;
}


bool Point::isZero() const
{
	return x == 0 && y == 0;
}


Point Point::operator/(int value) const
{
	return Point(x / value, y / value);
}


Point& Point::operator/=(int value)
{
	x /= value;
	y /= value;
	return *this;
}


Point Point::operator*(int value) const
{
	return Point(x * value, y * value);
}


Point& Point::operator*=(int value)
{
	x *= value;
	y *= value;
	return *this;
}


Point Point::operator+(const Point& other) const
{
	return Point(x + other.x, y + other.y);
}


Point& Point::operator+=(const Point& other)
{
	x += other.x;
	y += other.y;
	return *this;
}


Point Point::operator+(int value) const
{
	return Point(x + value, y + value);
}


Point& Point::operator+=(int value)
{
	x += value;
	y += value;
	return *this;
}


Point Point::operator-(const Point& other) const
{
	return Point(x - other.x, y - other.y);
}


Point& Point::operator-=(const Point& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}


Point Point::operator-(int value) const
{
	return Point(x - value, y - value);
}


Point& Point::operator-=(int value)
{
	x -= value;
	y -= value;
	return *this;
}


bool  Point::operator==(const Point& other) const
{
	return (x == other.x) && (y == other.y);
}


bool  Point::operator!=(const Point& other) const
{
	return !(operator==(other));
}


bool Point::operator<(const Point& other) const
{
	if (x != other.x)
		return x < other.x;
	return y < other.y;
}