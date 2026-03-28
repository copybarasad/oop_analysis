#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(int hp, int dmg);
    int hp() const;
    int damage() const;

    EntityType type() const override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

private:
    int damage_{};
};
