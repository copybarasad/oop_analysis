#include "cell.h"
#include <iostream>
#include <stdexcept>
#include <format>
#include <cstring>

#ifndef FIELD_H
#define FIELD_H

class Field
{
private:
	int height;
	int width;
	Cell** field;
	void fieldInit();
public:
	Field();
	Field(int height, int width);
	~Field();
	Field(const Field& other);
	Field& operator=(const Field& other);
	Field(Field&& other) noexcept;
	Field& operator=(Field&& other) noexcept;
	void clearField();

	int getHeight() const;
	int getWidth() const;
	Cell** getField() const;

	void setUnpassableCell(int x, int y);

	bool isCellPassable(int x, int y) const;

	//TRAP

	void setTrappedCell(int x, int y, int damage);
	bool isCellTrapped(int x, int y) const;
	int useTrap(int x, int y);

	//SETTERS
	void setField(Cell** field, int width, int height);
	void setWidth(int width);
	void setHeight(int height);
	void setCell(Cell cell);
};

#endif // FIELD_H