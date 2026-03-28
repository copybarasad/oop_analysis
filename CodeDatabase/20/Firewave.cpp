#include "../include/Firewave.h"

void Firewave::cast(std::string direction, Gamefield* gf, int pl_X, int pl_Y) {
    int radius = 3;

    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            int x = pl_X + dx;
            int y = pl_Y + dy;

            if (!gf->in_bounds(x, y)) continue;

            Cell* cell = gf->getCell(x, y);

            if (cell->is_occupied()) {
                Entity* entity = cell->is_occupied_by();
                Enemy* enemy = dynamic_cast<Enemy*>(entity);
                if (enemy) {
                    enemy->get_damaged(this->damage);
                    std::cout << "Firewave hit enemy at (" << x << ", " << y << ") for " 
                              << this->damage << " damage.\n";
                }
            }
        }
    }
}



//constructor
Firewave::Firewave() {
        setName("Firewave");
        setDamage(5);
        setRange(2);
    }


