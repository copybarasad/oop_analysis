#pragma once
#include "entity.hpp"

class Ally : public Entity {
public:
    bool skip_turn = false;
    Ally(int id);

    Ally(Ally&& other) noexcept;
    Ally& operator=(Ally&& other) noexcept;
    Ally(const Ally&) = delete;
    Ally& operator=(const Ally&) = delete;
};