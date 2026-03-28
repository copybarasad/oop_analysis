#include "Cell.h"

Cell::Cell(TypeOfCell type) 
	: type(type) {}


TypeOfCell Cell::GetTypeOfCell() const{
	return type;
}

int Cell::GetBuff() const{
	return buff;
}

void Cell::SetTypeOfCell(TypeOfCell newType)
{
	type = newType;
}

void Cell::SetBuff(int addition)
{
	buff += addition;
}

std::shared_ptr<Entity> Cell::GetCellContent()
{
	return cellContent;
}

const std::shared_ptr<Entity> Cell::GetCellContentForReading() const{
	return cellContent;
}

void Cell::ChangeCellContent(std::shared_ptr<Entity> newEntity, bool isClearing)
{
	if (!isClearing)
		cellContent = newEntity;
	else
		cellContent = nullptr;
}