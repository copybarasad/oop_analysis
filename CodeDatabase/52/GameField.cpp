#include "GameField.h"
#include "Enemy.h"
#include "Ally.h"

#include <cassert>
#include <queue>
#include <cstdlib>

GameField::GameField(int w, int h)
    : width_(w), height_(h), cells_(h, std::vector<Cell>(w)) {}

bool GameField::Inside(int x, int y) const noexcept {
    return x >= 0 && y >= 0 && x < width_ && y < height_;
}

Cell& GameField::CellAt(int x, int y) { return cells_[y][x]; }
const Cell& GameField::CellAt(int x, int y) const { return cells_[y][x]; }

void GameField::PlacePlayer(std::shared_ptr<Player> p, int x, int y) {
    assert(Inside(x,y));
    player_ = std::move(p);
    cells_[y][x].setEntity(player_);
}

std::pair<int,int> GameField::PlayerCell() const {
    if (!player_) return {-1,-1};
    for (int y=0; y<height_; ++y)
        for (int x=0; x<width_; ++x)
            if (cells_[y][x].getEntity().get() == player_.get())
                return {x,y};
    return {-1,-1};
}

std::pair<int,int> GameField::FindEntity(const Entity* who) const {
    for (int y=0; y<height_; ++y)
        for (int x=0; x<width_; ++x)
            if (cells_[y][x].hasEntity() &&
                cells_[y][x].getEntity().get() == who)
                return {x,y};
    return {-1,-1};
}

Enemy* GameField::EnemyAt(int x, int y) const noexcept {
    if (!Inside(x,y)) return nullptr;
    auto e = cells_[y][x].getEntity();
    return dynamic_cast<Enemy*>(e.get());
}

Ally* GameField::AllyAt(int x, int y) const noexcept {
    if (!Inside(x,y)) return nullptr;
    auto e = cells_[y][x].getEntity();
    return dynamic_cast<Ally*>(e.get());
}

bool GameField::TryMove(Entity* who, int dx, int dy) {
    auto [x,y] = FindEntity(who);
    if (x<0) return false;
    int nx = x+dx, ny = y+dy;
    if (!Inside(nx,ny)) return false;

    auto& from = cells_[y][x];
    auto& to   = cells_[ny][nx];

    if (!to.isWalkable()) return false;

    if (to.hasEntity()) {
        auto other = to.getEntity();

        other->TakeDamage(who->Damage());
        who->TakeDamage(other->Damage());

        if (!other->IsAlive())
            to.setEntity(nullptr);

        if (!who->IsAlive())
            from.setEntity(nullptr);

        return true;
    }

    auto me = from.getEntity();
    from.setEntity(nullptr);
    to.setEntity(me);

    if (to.hasTrap()) {
        if (dynamic_cast<Player*>(who) == nullptr) {
            int dmg = to.triggerTrap();
            who->TakeDamage(dmg);
        }
    }

    return true;
}

void GameField::PlaceTrap(int x, int y, int dmg) {
    if (Inside(x,y))
        cells_[y][x].placeTrap(dmg);
}

void GameField::SpawnAlly(int x, int y) {
    if (!Inside(x,y)) return;
    if (cells_[y][x].hasEntity()) return;
    cells_[y][x].setEntity(std::make_shared<Ally>());
}

void GameField::SpawnAlliesAround(int cx, int cy, int count) {
    static const int dirs[8][2] = {
        {1,0},{-1,0},{0,1},{0,-1},{1,1},{-1,1},{1,-1},{-1,-1}
    };
    int spawned = 0;
    for (auto& d : dirs) {
        if (spawned >= count) break;
        int x = cx + d[0], y = cy + d[1];
        if (!Inside(x,y)) continue;
        if (!cells_[y][x].hasEntity() && cells_[y][x].isWalkable()) {
            SpawnAlly(x,y);
            ++spawned;
        }
    }
}

void GameField::KillDead() {
    for (int y=0; y<height_; ++y)
        for (int x=0; x<width_; ++x)
            if (cells_[y][x].hasEntity() &&
                !cells_[y][x].getEntity()->IsAlive())
                cells_[y][x].setEntity(nullptr);
}

std::vector<std::pair<int,int>> GameField::ListEnemies() const {
    std::vector<std::pair<int,int>> res;
    for (int y=0; y<height_; ++y)
        for (int x=0; x<width_; ++x)
            if (dynamic_cast<Enemy*>(cells_[y][x].getEntity().get()))
                res.emplace_back(x,y);
    return res;
}

bool GameField::AnyEnemiesAlive() const {
    for (int y=0; y<height_; ++y)
        for (int x=0; x<width_; ++x)
            if (dynamic_cast<Enemy*>(cells_[y][x].getEntity().get()))
                return true;
    return false;
}

void GameField::UpdateAll() {
    std::vector<std::shared_ptr<Entity>> entities;

    for (int y=0; y<height_; ++y)
        for (int x=0; x<width_; ++x)
            if (cells_[y][x].hasEntity())
                entities.push_back(cells_[y][x].getEntity());

    for (auto& e : entities) {
        if (!e->IsAlive()) continue;
        if (dynamic_cast<Player*>(e.get())) continue;
        e->Update(*this);
    }

    KillDead();
}

bool GameField::PlayerHasPath() const {
    auto [sx, sy] = PlayerCell();
    if (sx < 0) return true;

    bool anyEnemies = false;
    for (int y=0; y<height_; ++y)
        for (int x=0; x<width_; ++x)
            if (EnemyAt(x,y))
                anyEnemies = true;

    std::vector<std::vector<bool>> visited(height_, std::vector<bool>(width_, false));
    std::queue<std::pair<int,int>> q;
    q.push({sx, sy});
    visited[sy][sx] = true;

    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    while (!q.empty()) {
        auto [x,y] = q.front(); q.pop();
        if (EnemyAt(x,y)) return true;
        for (auto& d : dirs) {
            int nx = x + d[0], ny = y + d[1];
            if (!Inside(nx,ny)) continue;
            if (visited[ny][nx]) continue;
            if (!cells_[ny][nx].isWalkable()) continue;
            visited[ny][nx] = true;
            q.push({nx,ny});
        }
    }
    if (!anyEnemies) return true;
    return false;
}

void GameField::GenerateWalls(float density) {
    int total = width_ * height_;
    int wallCount = total * density;

    for (;;) {
        for (int y=0; y<height_; ++y)
            for (int x=0; x<width_; ++x)
                cells_[y][x].setWalkable(true);

        int placed = 0;
        while (placed < wallCount) {
            int x = std::rand() % width_;
            int y = std::rand() % height_;
            auto [px,py] = PlayerCell();
            if (x==px && y==py) continue;
            if (cells_[y][x].hasEntity()) continue;
            cells_[y][x].setWalkable(false);
            ++placed;
        }

        if (PlayerHasPath()) break;
    }
}


void GameField::SetEntity(int x, int y, std::shared_ptr<Entity> e) {
    cells_[y][x].setEntity(std::move(e));
}

void GameField::SetTrap(int x, int y, int dmg) {
    cells_[y][x].placeTrap(dmg);
}