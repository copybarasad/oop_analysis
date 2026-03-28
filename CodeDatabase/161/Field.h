#ifndef FIELD_H
#define FIELD_H

#include <random>
#include <vector>

#include "./cells/Cell.h"
#include "./cells/SlowingCell.h"
#include "./cells/ImpassableCell.h"
#include "../entites/Enemy.h"

enum class MoveType {
	Forward,
	Back,
	Left,
	Right
};


class Field {
public:
	Field();

	Field(int, int);

	Field(int, int, int, int);

	Field(const Field &);

	Field(Field &&);

	Field &operator=(const Field&);

	Field &operator=(Field&&);

	MoveResult MoveEntity(MoveType, std::shared_ptr<Entity>);

	bool SetEntity(std::shared_ptr<Entity>, int, int);

	bool RemoteEntity(std::shared_ptr<Entity>);

	int GetCountSpawnCells();

	void GetSize(int&, int&);

	bool GetPosEntity(std::shared_ptr<Entity>, int &, int &);

	bool CorrectPosition(int, int) const;

	std::shared_ptr<Entity> GetEntity(int, int) const;
	std::shared_ptr<Cell>  GetCell (int, int);

	std::string GetCellType(int, int) const;
	int getNotFriendlyCount();
	bool ReplaceCellWithType(int x, int y, const std::string& type);

private:
	int height_;
	int width_;
	int count_spawn_cells_;
	std::vector<std::vector<std::shared_ptr<Cell>>> field_;

	void CreateField(int, int);
};

#endif //FIELD_H
