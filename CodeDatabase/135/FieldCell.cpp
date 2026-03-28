#include "FieldCell.h"
#include "Trap.h"

FieldCell::FieldCell(CellType cell_type)
	: type(cell_type), trap(nullptr) {}

FieldCell::~FieldCell() = default;

FieldCell::FieldCell(const FieldCell& other)
	: type(other.type), trap(nullptr) {
	if (other.trap) {
		trap = std::make_unique<Trap>(*other.trap);
	}
}

FieldCell& FieldCell::operator=(const FieldCell& other) {
	if (this != &other) {
		type = other.type;
		trap.reset();
		if (other.trap) {
			trap = std::make_unique<Trap>(*other.trap);
		}
	}
	return *this;
}

FieldCell::FieldCell(FieldCell&& other) noexcept = default;

FieldCell& FieldCell::operator=(FieldCell&& other) noexcept = default;

bool FieldCell::isPassable() const {
	return type != CellType::IMPASSABLE;
}

bool FieldCell::isSlowing() const {
	return type == CellType::SLOWING;
}

CellType FieldCell::getType() const {
	return type;
}

void FieldCell::setTrap(std::unique_ptr<Trap> new_trap) {
	trap = std::move(new_trap);
}

bool FieldCell::hasTrap() const {
	return trap != nullptr;
}

Trap* FieldCell::getTrap() const {
	return trap.get();
}

std::unique_ptr<Trap> FieldCell::removeTrap() {
	return std::move(trap);
}
