#include "SummonSpell.h"
#include "GameController.h"
#include <iostream>

SummonSpell::SummonSpell(int cnt) : count(cnt) {}

bool SummonSpell::apply(GameController &controller, const Position &casterPosition, const Position &targetPos)
{
    int created = 0;
    for (int dx=-1; dx<=1 && created<count; ++dx) {
        for (int dy=-1; dy<=1 && created<count; ++dy) {
            Position p(casterPosition.x+dx, casterPosition.y+dy);
            if (!controller.getField().isValidPosition(p)) continue;
            if (p==casterPosition) continue;
            controller.addAlly(Ally(p, 50, 10));
            ++created;
            std::cout << "Summoned ally at ("<<p.x<<","<<p.y<<")\n";
        }
    }
    if (created==0) std::cout << "Summon: no space to summon.\n";
    return created>0;
}