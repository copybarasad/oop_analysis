#pragma once
#include "Coords.hpp"

class Trap {
public:
	Trap(const Coords& pos, int damage);
	Trap(const Trap& other);
	Trap(Trap&& other) noexcept;
	Trap& operator=(const Trap& other);
	Trap& operator=(Trap&& other) noexcept;
	~Trap() = default;

	const Coords& getPosition() const noexcept;
	int getDamage() const noexcept;
	bool isActive() const noexcept;
	
	int trigger();

private:
	Coords position;
	int damage;
	bool active;
};


