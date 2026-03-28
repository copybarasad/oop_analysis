#pragma once
#include "Entity.hpp"
#include "./Field/Cell.hpp"

class Knight : public Entity {
public:
    Knight(int startX, int startY, int offsetX, int offsetY, int level = 1);
    void move_towards_target(Field& field, InterfaceGameLogger& logger_consol) override;
    void random_move(Field& field, InterfaceGameLogger& logger_consol);
};