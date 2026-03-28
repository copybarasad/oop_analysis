#include "SummonSpell.h"
#include "Field.h"
#include "Player.h"
#include "Ally.h"

SummonSpell::SummonSpell(int count) : summonCount_(count) {}

std::string SummonSpell::getName() const {
    return "Summon Ally";
}

bool SummonSpell::apply(Field& field, const Player& player, int targetX, int targetY) {
    auto [px, py] = field.findPlayer();
    std::vector<std::pair<int, int>> dirs = {{1,0},{-1,0},{0,1},{0,-1}};
    int placed = 0;
    for (auto [dx, dy] : dirs) {
        int nx = px + dx, ny = py + dy;
        if (field.inBounds(nx, ny) && field.getCell(nx, ny).isEmpty()) {
            field.placeAlly(nx, ny, std::make_shared<Ally>());
            placed++;
            if (placed >= summonCount_) break;
        }
    }
    return placed > 0;
}