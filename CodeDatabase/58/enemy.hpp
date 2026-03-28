#pragma once

#include "entity.hpp"

class Enemy : public Entity{
public:
    using Entity::Entity;
    virtual ~Enemy() override = default;

    virtual char get_symbol() override;
};