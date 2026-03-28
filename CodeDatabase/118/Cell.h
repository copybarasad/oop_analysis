#ifndef CELL_H
#define CELL_H

#include "Entity.h"
#include "Constants.h"
#include <memory>

class Cell
{
	TypeOfCell type;
	std::shared_ptr<Entity> cellContent;
	int buff = 0;
public:
	Cell(TypeOfCell type = ORDINAL);
	TypeOfCell GetTypeOfCell()const;
	int GetBuff()const;
	void SetTypeOfCell(TypeOfCell);
	void SetBuff(int);
	const std::shared_ptr<Entity> GetCellContentForReading()const;
	std::shared_ptr<Entity> GetCellContent();
	void ChangeCellContent(std::shared_ptr<Entity>, bool isClearing=false);
};

#endif

