#ifndef FIELD_CELL_H
#define FIELD_CELL_H

#include <memory>

class Trap;

enum class CellType {
	NORMAL,
	IMPASSABLE,
	SLOWING
};

class FieldCell {
private:
	CellType type;
	std::unique_ptr<Trap> trap;

public:
	explicit FieldCell(CellType cell_type = CellType::NORMAL);
	~FieldCell();
	
	FieldCell(const FieldCell& other);
	FieldCell& operator=(const FieldCell& other);
	
	FieldCell(FieldCell&& other) noexcept;
	FieldCell& operator=(FieldCell&& other) noexcept;
	
	bool isPassable() const;
	bool isSlowing() const;
	CellType getType() const;
	
	void setTrap(std::unique_ptr<Trap> new_trap);
	bool hasTrap() const;
	Trap* getTrap() const;
	std::unique_ptr<Trap> removeTrap();
};

#endif
