#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "FieldCell.h"
#include "RandomGenerator.h"
#include <vector>

class GameField {
private:
	std::vector<FieldCell> cells;
	int width_field;
	int height_field;
	
	void initializeCells(RandomGenerator& rng);

public:
	GameField(int width, int height);
	GameField(int width, int height, RandomGenerator& rng);
	
	static GameField createEmpty(int width, int height);
	
	GameField(const GameField& other) = default;
	
	GameField(GameField&& other) noexcept = default;
	
	GameField& operator=(const GameField& other) = default;
	
	GameField& operator=(GameField&& other) noexcept = default;
	
	int coordToIndex(int x, int y) const;
	void indexToCoord(int index, int& x, int& y) const;
	bool isValidPosition(int x, int y) const;
	
	FieldCell& getCell(int index);
	const FieldCell& getCell(int index) const;
	
	void setCellType(int index, CellType type);
	
	int getWidth() const;
	int getHeight() const;
};

#endif
