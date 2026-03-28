#include "AreaSpell.h"
#include "Game.h"

std::vector<std::pair<int, int> > AreaSpell::get_targeted_cells(std::pair<int, int> target) {
    std::vector<std::pair<int, int>> cells;
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            cells.emplace_back(target.first + dx, target.second + dy);
        }
    }
    return cells;
}

bool AreaSpell::use(Game &game, std::pair<int, int> caster, std::pair<int, int> target) {
    if (!checkTarget(caster, target)) return false;

    bool hit = false;
    for (auto cell : get_targeted_cells(target)) {
        if (caster == cell) continue;
        hit += game.attack_cell(*this, cell);
    }
    return true; 
}
