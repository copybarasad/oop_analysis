#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include "Cell.h"

class GameField
{
	std::vector<std::vector<Cell>> cells;
	int width, height;

public:
	GameField(int=10, int=10);
	GameField(const GameField&);
	GameField& operator = (const GameField&);
	GameField(GameField&&)noexcept;
	GameField& operator = (GameField&&)noexcept;
	int GetWidth()const;
	int GetHeight()const;
	Cell& GetCellByPos(std::pair<int, int>);
	const Cell& GetCellByPosForReading(std::pair<int, int>)const;
	void EditCellByPos(std::shared_ptr<Entity>, std::pair<int, int>, bool isClearing=false);
	void SwapCellsContent(std::pair<int, int>, std::pair<int, int>);
	bool IsCorrectPos(std::pair<int, int>)const;
};

#endif

