#include "PetController.h"
#include <random>
#include "Cell.h"

void PetController::processPetTurn() {
    auto& pets = field->getPets();
    if (pets.empty()) return;
    std::random_device rd; std::mt19937 gen(rd());
    std::uniform_int_distribution<> dir(0, 3);
    const int dxs[4] = {1,-1,0,0};
    const int dys[4] = {0,0,1,-1};
    for (auto& pet : pets) {
        int x = pet.getX();
        int y = pet.getY();
        for (int tries = 0; tries < 4; ++tries) {
            int i = dir(gen);
            int nx = x + dxs[i];
            int ny = y + dys[i];
            if (!field->isValidPosition(nx, ny)) continue;
            if (!field->isPositionPassable(nx, ny)) continue;
            field->getCells()[y][x].setHasEntity(false);
            field->getCells()[ny][nx].setHasEntity(true);
            pet.setPosition(nx, ny);
            break;
        }
    }
}



