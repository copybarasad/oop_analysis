#pragma once
#include "../../saving/Saving.h"

class Cell{
protected:
    bool impassible;
    bool is_there_player;
    bool is_there_enemy;
    bool trap;
    bool enemy_tower;
public:
    Cell();

    void set_player(bool value);
    void set_enemy(bool value);
    void set_trap(bool value);
    void set_impassible(bool value);
    void set_enemy_tower(bool value);

    bool is_player();
    bool is_enemy();
    bool is_impassible();
    bool is_trap();
    bool is_enemy_tower();

    Saving::CellData serialize() const;
    void deserialize(const Saving::CellData& data);

    ~Cell();
};
