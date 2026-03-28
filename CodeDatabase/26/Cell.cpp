#include "Cell.hpp"

Cell::Cell(Type type) : type_(type) {}

Cell::Type Cell::type() const noexcept {
	return type_;
}

bool Cell::isPassable() const noexcept {
	return type_ != Type::Wall;
}

bool Cell::isSlow() const noexcept {
	return type_ == Type::Slow;
}
