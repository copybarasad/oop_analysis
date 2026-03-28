#include "GameView.h"
#include "GameModel.h"
#include "Field.h"
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Spawner.h"
#include "Trap.h"
#include "Ally.h"

#include <iostream>
#include <sstream>

void GameView::render(const GameModel& model) const {
    const Field& field = model.field();

    int rows = field.rows();
    int cols = field.cols();
    int maxRowIdx = rows - 1;
    int maxColIdx = cols - 1;
    int rowIdxWidth = 0;
    int tmp = maxRowIdx;
    do { ++rowIdxWidth; tmp /= 10; } while (tmp > 0);

    for (int i = 0; i < rowIdxWidth; ++i) std::cout << ' ';
    std::cout << ' ';
    for (int c = 0; c < cols; ++c) {
        if (c < 10) std::cout << c;
        else std::cout << c;
        if (c != cols - 1) std::cout << ' ';
    }
    std::cout << '\n';

    for (int r = 0; r < rows; ++r) {
        std::ostringstream rowLabel;
        rowLabel << r;
        std::string rl = rowLabel.str();
        for (int i = 0; i < rowIdxWidth - (int)rl.size(); ++i) std::cout << ' ';
        std::cout << rl << ' ';

        for (int c = 0; c < cols; ++c) {
            const auto &cell = field.cellAt({r, c});
            if (!cell.hasEntity()) {
                if (!cell.isPassable()) std::cout << '#';
                else if (cell.isSlow()) std::cout << '~';
                else std::cout << '.';
            } else {
                auto ent = cell.getEntity();
                if (dynamic_cast<Player*>(ent.get())) std::cout << 'P';
                else if (dynamic_cast<EnemyBuilding*>(ent.get())) std::cout << 'B';
                else if (dynamic_cast<Spawner*>(ent.get())) std:: cout << 'S';
                else if (dynamic_cast<Trap*>(ent.get())) std::cout << '^';
                else if (dynamic_cast<Ally*>(ent.get())) std::cout << 'A';
                else if (dynamic_cast<Enemy*>(ent.get())) std::cout << 'E';
                else std::cout << '?';
            }
            if (c != cols - 1) std::cout << ' ';
        }
        std::cout << '\n';
    }
}
