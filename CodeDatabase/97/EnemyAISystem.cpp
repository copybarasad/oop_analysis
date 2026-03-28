#include "EnemyAISystem.h"
#include "GameModel.h"
#include "Field.h"
#include "Entity.h"
#include "Enemy.h"
#include "Ally.h"
#include "MovementSystem.h"
#include <climits>

void EnemyAISystem::step(GameModel& model, MovementSystem& mover) {
    Field& field = model.field();
    std::vector<Coord> movers;

    for (int r = 0; r < field.rows(); ++r) {
        for (int c = 0; c < field.cols(); ++c) {
            auto ent = field.cellAt({r,c}).getEntity();
            if (ent && dynamic_cast<Enemy*>(ent.get())) movers.push_back({r,c});
        }
    }

    for (auto pos : movers) {
        auto entPtr = field.cellAt(pos).getEntity();
        if (!entPtr) continue;
        Ally* asAlly = dynamic_cast<Ally*>(entPtr.get());
        if (asAlly) {
            std::optional<Coord> best;
            int bestDist = INT_MAX;
            for (int r = 0; r < field.rows(); ++r) {
                for (int c = 0; c < field.cols(); ++c) {
                    auto e = field.cellAt({r,c}).getEntity();
                    if (e && dynamic_cast<Enemy*>(e.get()) && !dynamic_cast<Ally*>(e.get())) {
                        int d = std::abs(r - pos.row) + std::abs(c - pos.col);
                        if (d < bestDist) { bestDist = d; best = Coord{r,c}; }
                    }
                }
            }
            if (!best) continue;
            Coord target = *best;
            Coord dst = pos;
            if (pos.row < target.row) dst.row += 1;
            else if (pos.row > target.row) dst.row -= 1;
            else if (pos.col < target.col) dst.col += 1;
            else if (pos.col > target.col) dst.col -= 1;
            mover.moveEntity(pos, dst);
            continue;
        }

        auto pc = model.playerCoord();
        if (!pc) continue;
        Coord playerNow = *pc;
        Coord dst = pos;
        if (pos.row < playerNow.row) dst.row += 1;
        else if (pos.row > playerNow.row) dst.row -= 1;
        else if (pos.col < playerNow.col) dst.col += 1;
        else if (pos.col > playerNow.col) dst.col -= 1;
        auto [moved, playerDied, towerDestroyed, enemyKilled, spawnerKilled] = mover.moveEntity(pos, dst);
        (void)moved; (void)playerDied; (void)towerDestroyed;
        if (enemyKilled) {
        }
    }
}
