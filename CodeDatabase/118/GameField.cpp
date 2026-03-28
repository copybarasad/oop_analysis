#include "GameField.h"
#include <random>
#include <iostream>

GameField::GameField(int w, int h)
	: width(w), height(h)
{
	cells = std::vector<std::vector<Cell>>(height, std::vector<Cell>(width, Cell(TypeOfCell::ORDINAL)));
}

GameField::GameField(const GameField& source)
	: width(source.width), height(source.height)
{
	
	cells.resize(height, std::vector<Cell>(width));
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
			cells[y][x] = source.cells[y][x];
	}
}

GameField& GameField::operator=(const GameField& source)
{
	if (this != &source)
	{
		width = source.width;
		height = source.height;

		cells.clear();
		
		cells.resize(height, std::vector<Cell>(width));
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
				cells[y][x] = source.cells[y][x];
		}

	}
	return *this;
}

GameField::GameField(GameField&& other)noexcept
	: width(other.width), height(other.height), cells(std::move(other.cells)){
	other.width = 0;
	other.height = 0;
}

GameField& GameField::operator=(GameField&& other) noexcept{
	if (this != &other)
	{
		this->width = other.width;
		this->height = other.height;
		this->cells = std::move(other.cells);

		other.width = 0;
		other.height = 0;
	}
	return *this;
}

int GameField::GetWidth() const{
	return width;
}

int GameField::GetHeight() const{
	return height;
}

Cell& GameField::GetCellByPos(std::pair<int, int> pos)
{
	return cells[pos.second][pos.first];
}

const Cell& GameField::GetCellByPosForReading(std::pair<int, int> pos) const{
	return cells[pos.second][pos.first];
}

void GameField::EditCellByPos(std::shared_ptr<Entity> entity, std::pair<int, int> pos, bool isClearing)
{
	cells[pos.second][pos.first].ChangeCellContent(entity, isClearing);
}

void GameField::SwapCellsContent(std::pair<int, int> pos1, std::pair<int, int> pos2)
{
	if (cells[pos1.second][pos1.first].GetCellContent() != nullptr || cells[pos2.second][pos2.first].GetCellContent() != nullptr)
	{
		auto tmp = cells[pos1.second][pos1.first].GetCellContent();
		EditCellByPos(cells[pos2.second][pos2.first].GetCellContent(), pos1);
		EditCellByPos(tmp, pos2);
	}
}

bool GameField::IsCorrectPos(std::pair<int, int> pos) const{
	return pos.first >= 0 && pos.first < width && pos.second >= 0 && pos.second < height;
}