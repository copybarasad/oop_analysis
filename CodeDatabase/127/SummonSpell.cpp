// SummonSpell.cpp
#include "SummonSpell.h"
#include "Game.h"
#include <algorithm>
#include <string> 

SummonSpell::SummonSpell(int summons, int price) : summons_(summons), price_(price) {}

int SummonSpell::cost() const { return price_; }

bool SummonSpell::apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage) {
    int total = summons_ + enh.extraSummons;
    if (total < 1) total = 1;
    int px = player.getX();
    int py = player.getY();
    int placed = 0;
    for (int dy = -1; dy <= 1 && placed < total; ++dy) {
        for (int dx = -1; dx <= 1 && placed < total; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = px + dx;
            int ny = py + dy;
            if (game.isCellEmptyForMove(nx, ny)) {
                if (game.addAlly(nx, ny)) {
                    ++placed;
                }
            }
        }
    }
    return (placed > 0);
}
std::string SummonSpell::getDescription() const {
    return "Призывает " + std::to_string(summons_) + " союзника(ов) рядом с игроком.";
}