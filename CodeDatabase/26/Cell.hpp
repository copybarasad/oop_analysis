#pragma once

class Cell {
public:
	enum class Type { Floor, Wall, Slow };
	explicit Cell(Type type = Type::Floor);
	Type type() const noexcept;
	bool isPassable() const noexcept;
	bool isSlow() const noexcept;

private:
	Type type_;
};
