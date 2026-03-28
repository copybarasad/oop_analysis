#pragma once
#include "Entity.h"

class Ally : public Entity {
public:
	Ally(int hp, int dmg);
	int damage() const;

	EntityType type() const override;

	void save(std::ostream& os) const override;
	void load(std::istream& is) override;

private:
	int damage_{};
};
