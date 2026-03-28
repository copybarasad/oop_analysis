#include "../include/Lightning_bolt.h"

//constructor

Lightning_bolt::Lightning_bolt() {
        setName("Lightning Bolt");
        setDamage(10);
        setRange(3);
    }


//functions

void Lightning_bolt::cast(const std::string direction, Gamefield* gf, int x, int y)
{
    int dx = 0, dy = 0;

    if (direction == "up") dy = -1;
    else if (direction == "down") dy = 1;
    else if (direction == "left") dx = -1;
    else if (direction == "right") dx = 1;

    int cx = x;
    int cy = y;

    while (true) {
        cx += dx;
        cy += dy;

        Cell* cell = gf->getCell(cx, cy);
        if (!cell) break; 

        if (cell->is_occupied()) {
            Entity* e = cell->is_occupied_by();
            if (e) {
                e->get_damaged(this->getDamage());

                if (!e->isAlive()) {
                    cell->set_occupied(false);
                    cell->set_occupied_by(nullptr);
                }
            }
        }

    }
}

