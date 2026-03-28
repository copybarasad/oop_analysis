#pragma once


class Point
{
private:
	int x;

	int y;

public:
	Point();

	Point(int x, int y);

	void setX(int x);

	void setY(int y);

	int X() const;

	int Y() const;

	bool isZero() const;

	Point operator*(int value) const;

	Point& operator*=(int value);

	Point operator/(int value) const;

	Point& operator/=(int value);

	Point operator+(const Point& other) const;

	Point& operator+=(const Point& other);

	Point operator+(int value) const;

	Point& operator+=(int value);

	Point operator-(const Point& other) const;

	Point& operator-=(const Point& other);

	Point operator-(int value) const;

	Point& operator-=(int value);

	bool operator==(const Point& other) const;

	bool operator!=(const Point& other) const;

	bool operator<(const Point& other) const;
};