#pragma once
#include <vector>
#include "BasicSpell.h"

class DamagingSpell : public BasicSpell {
    public:
    DamagingSpell(Type type, int damage, float range, std::string name) :
    BasicSpell(type, damage, range, name) {}

    std::vector<Coordinates> getAttackedCells(Coordinates target_pos);
    json toJson() const override;
    void fromJson(const json& j) override;
};