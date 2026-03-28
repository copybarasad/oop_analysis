#include "../include/Trap.h"



void TrapSpell::cast(std::string direction, Gamefield* gf, int pl_X, int pl_Y) {
    if (direction != "up" && direction != "down" && direction != "left" && direction != "right") {
        std::cout << "Unknown direction: " << direction << "\n";
        return;
    }

    Cell* target = nullptr;

    if (direction == "up") {
        target = gf->getCell(pl_X, pl_Y - 1);
    } else if (direction == "down") {
        target = gf->getCell(pl_X, pl_Y + 1);
    } else if (direction == "left") {
        target = gf->getCell(pl_X - 1, pl_Y);
    } else if (direction == "right") {
        target = gf->getCell(pl_X + 1, pl_Y);
    }

    if (target == nullptr) {
        std::cout << "Target cell is out of bounds!\n";
        return;
    }

    if (target->is_occupied()) {
        Entity* entity = target->is_occupied_by();
        Enemy* enemy = dynamic_cast<Enemy*>(entity);
        if (enemy) {
            std::cout << "Trap triggered immediately! Enemy on cell ("
                      << target->x_position() << ", " << target->y_position() << ")\n";
            enemy->get_damaged(this->getDamage());
            return; 
        }
    }


    target->set_traped(true);
    std::cout << "Cell (" << target->x_position() << ", "
              << target->y_position() << ") was trapped!\n";
}

TrapSpell::TrapSpell() {
    this->setDamage(10);
    this->setName("TrapSpell");
    this->setRange(1);
}