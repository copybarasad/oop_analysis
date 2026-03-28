#pragma once
#include "Entity.hpp"
#include "./Field/Cell.hpp"
#include "./Systems/InterfaceGameLogger.hpp"

class Enemy : public Entity {
public:
    Enemy(std::string puth_enemy_png, int startX, int startY, int offsetX, int offsetY, int atack,
    int helpth, std::string name, int level);

    void random_move(Field& field, InterfaceGameLogger& logger_consol);
    void move_towards_target(Field& field, InterfaceGameLogger& logger_consol) override;
};