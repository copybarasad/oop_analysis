#pragma once

#include <memory>
#include <stdexcept>

// #include "IEntity.hpp"
#include "field_handler.hpp"
#include "weapon.hpp"


class AttackHandler {
private:
    FieldHandler& field_handler;
public:
    AttackHandler(FieldHandler& field_handler);
    bool attack(const Position& target, Weapon weapon);
};