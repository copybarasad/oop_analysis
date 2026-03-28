#pragma once
#include "../game/manager/Manager.h"
#include "../ui/GField.h"
#include "../game/utils/Values.h"
#include "../command_manager/Control.h"

class NC_Graphic{
public:
    void draw(GField gfield);
    void display(Manager manager);

    void display_lifes(Manager manager);
    void display_combat(Manager manager);
    void display_moves(Manager manager);
    void display_controls();
    void display_spells_controls();
    void display_my_spells(Manager manager);
    void display_enemy_tower(Manager manager);
    void display_killed(Manager manager);
    void display_need_to_do(Manager manager);
};
