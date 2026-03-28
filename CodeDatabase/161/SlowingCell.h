#ifndef SLOWINGCELL_H
#define SLOWINGCELL_H

#include "Cell.h"

class SlowingCell : public Cell {
public:
	MoveResult EnterEntity(std::shared_ptr<Entity>) override;

	std::shared_ptr<Cell> cloneStructure() const override;
	std::string GetName() const override;

};

#endif //SLOWINGCELL_H