#include "Ally.h"
#include "Game.h"
#include "GameField.h"
#include "Cell.h"
#include "Enemy.h"

#include <limits>
#include <cmath>
#include <queue>
#include <vector>

void Ally::Move(Game &game, std::size_t x, std::size_t y)
{
    if (!IsAlive()) {
        return;
    }

    GameField &field = game.GetField();
    GameField::Size size = field.GetSize();
    auto &enemies = game.GetEnemies();

    const std::size_t width  = size.width;
    const std::size_t height = size.height;

    const int dirs[4][2] = {
        { 1, 0},
        {-1, 0},
        { 0, 1},
        { 0,-1}
    };

    for (auto &d : dirs) {
        int nx_i = static_cast<int>(x) + d[0];
        int ny_i = static_cast<int>(y) + d[1];

        if (nx_i < 0 || ny_i < 0 ||
            nx_i >= static_cast<int>(width) ||
            ny_i >= static_cast<int>(height)) {
            continue;
        }

        std::size_t nx = static_cast<std::size_t>(nx_i);
        std::size_t ny = static_cast<std::size_t>(ny_i);

        if (field.GetOccupant(nx, ny) == Cell::Occupant::kEnemy) {
            int enemy_index = field.GetOccupantIndex(nx, ny);
            if (enemy_index >= 0 &&
                static_cast<std::size_t>(enemy_index) < enemies.size() &&
                enemies[static_cast<std::size_t>(enemy_index)].IsAlive())
            {
                enemies[static_cast<std::size_t>(enemy_index)].TakeDamage(damage_);
            }
            return;
        }
    }

    std::vector<std::vector<int>> dist(
        height, std::vector<int>(width, -1)
    );
    std::vector<std::vector<std::pair<int,int>>> parent(
        height, std::vector<std::pair<int,int>>(width, {-1, -1})
    );

    std::queue<std::pair<int,int>> q;

    int start_x = static_cast<int>(x);
    int start_y = static_cast<int>(y);

    dist[start_y][start_x] = 0;
    parent[start_y][start_x] = {-1, -1};
    q.push({start_x, start_y});

    auto can_stand = [&](int tx, int ty) -> bool {
        if (tx < 0 || ty < 0 ||
            tx >= static_cast<int>(width) ||
            ty >= static_cast<int>(height)) {
            return false;
        }

        std::size_t sx = static_cast<std::size_t>(tx);
        std::size_t sy = static_cast<std::size_t>(ty);

        if (field.GetTerrain(sx, sy) == Cell::Terrain::kBlocked) {
            return false;
        }

        if (field.GetOccupant(sx, sy) != Cell::Occupant::kNone &&
            !(tx == start_x && ty == start_y)) {
            return false;
        }

        return true;
    };

    // --- BFS по свободным клеткам ---
    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        for (auto &d : dirs) {
            int nx = cx + d[0];
            int ny = cy + d[1];

            if (!can_stand(nx, ny)) {
                continue;
            }

            if (dist[ny][nx] != -1) {
                continue;
            }

            dist[ny][nx] = dist[cy][cx] + 1;
            parent[ny][nx] = {cx, cy};
            q.push({nx, ny});
        }
    }

    bool found_target = false;
    int best_dist = std::numeric_limits<int>::max();
    int best_tx = -1;
    int best_ty = -1;

    for (std::size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].IsAlive()) {
            continue;
        }

        std::size_t ex = 0, ey = 0;
        if (!game.FindEnemyPosition(i, ex, ey)) {
            continue;
        }

        int ex_i = static_cast<int>(ex);
        int ey_i = static_cast<int>(ey);

        for (auto &d : dirs) {
            int nx = ex_i + d[0];
            int ny = ey_i + d[1];

            if (nx < 0 || ny < 0 ||
                nx >= static_cast<int>(width) ||
                ny >= static_cast<int>(height)) {
                continue;
            }

            if (dist[ny][nx] <= 0) {
                continue;
            }

            if (dist[ny][nx] < best_dist) {
                best_dist = dist[ny][nx];
                best_tx = nx;
                best_ty = ny;
                found_target = true;
            }
        }
    }

    if (!found_target) {
        return;
    }

    int cur_x = best_tx;
    int cur_y = best_ty;

    while (dist[cur_y][cur_x] > 1) {
        auto [px, py] = parent[cur_y][cur_x];
        cur_x = px;
        cur_y = py;
    }

    std::size_t step_x = static_cast<std::size_t>(cur_x);
    std::size_t step_y = static_cast<std::size_t>(cur_y);

    if (!can_stand(cur_x, cur_y)) {
        return;
    }

    int ally_index = field.GetOccupantIndex(x, y);

    field.ClearCell(x, y);
    field.SetOccupant(step_x, step_y, Cell::Occupant::kAlly, ally_index);
}