#ifndef IMPASSABLECELL_H
#define IMPASSABLECELL_H

#include "Cell.h"

class ImpassableCell : public Cell {
public:
	ImpassableCell();
	MoveResult EnterEntity(std::shared_ptr<Entity>) override;
	bool SpawnEntity(std::shared_ptr<Entity>) { return false; }
	std::string GetName() const override;

	std::shared_ptr<Cell> cloneStructure() const override;

	bool SetEvent(std::shared_ptr<IEvent>);

private:
	ImpassableCell(std::shared_ptr<Entity>) = delete;
};

#endif //IMPASSABLECELL_H