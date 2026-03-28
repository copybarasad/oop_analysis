#pragma once
#include "Enemy.h"
#include "Player.h"
#include "Tower.h"
#include "JsonLib/json.hpp"
#include <vector>
#include <set>


class Field
{
public:
	class Cell
	{
	private:
		bool empty = true;
		bool slow = false;
		bool obstruction = true;
	public:
		void setSlow(bool value);

		void setObstruction(bool value);

		bool isEmpty() const;

		bool isSlow() const;

		bool isObstruction() const;
	};
private:
	int width;
	int height;
	int cellSize;
	std::vector<std::vector<Cell>> field;
public:
	Field(int width, int height, int cellSize);

	Field(const Field& other);

	Field(Field&& other) noexcept;

	void to_json(nlohmann::json& js) const;

	void from_json(const nlohmann::json& js);

	Field& operator=(const Field& other);
	
	Field& operator=(Field&& other) noexcept;

	int getWidth(bool realSize = false) const;

	int getHeight(bool realSize = false) const;

	int getCellSize() const;

	const Cell& getCell(const Point& point) const;

	void generateSlowCells(int n);

	void generateObstructionCells(int n = -1);

	void changeObstruction(const Point& point);

	const std::vector<std::vector<Cell>>& getField() const;

	std::set<Point> generatePositions(const Point& p, int n, int radius);

	std::set<Point> generatePositions(int n, bool escapeNotEmpty = false);

	~Field() = default;

};