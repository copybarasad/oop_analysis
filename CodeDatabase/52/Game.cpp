#include "Game.h"
#include "SpellFactory.h"
#include "Enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

static int clampDim(int v) {
    if (v < 10) return 10;
    if (v > 25) return 25;
    return v;
}

Game::Game(int w, int h)
    : field_(clampDim(w), clampDim(h)) 
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    player_ = std::make_shared<Player>("Hero", 30, 5, 4);

    int px = field_.Width()/2;
    int py = field_.Height()/2;
    field_.PlacePlayer(player_, px, py);

    int enemies = 4 + std::rand() % 3;
    for (int i = 0; i < enemies; ++i) {
        int x = std::rand() % field_.Width();
        int y = std::rand() % field_.Height();
        if (x == px && y == py) { --i; continue; }
        if (!field_.CellAt(x,y).hasEntity() && field_.CellAt(x,y).isWalkable())
            field_.CellAt(x,y).setEntity(std::make_shared<Enemy>());
        else
            --i;
    }

    field_.GenerateWalls(0.10f);

    player_->HandRef().Add(SpellFactory::MakeRandom());
}

void Game::Init() { }

void Game::MovePlayer(int dx, int dy) {
    field_.TryMove(player_.get(), dx, dy);
}

void Game::PlayerAttack() {
    player_->PerformAttack(field_);
}

void Game::PlayerSwitchMode() {
    player_->SwitchMode();
}

void Game::PlayerCastSpell() {
    std::cout << "Hand:\n";
    for (size_t i=0; i<player_->HandRef().Size(); ++i) {
        if (auto* sp = player_->HandRef().Get(i))
            std::cout << " [" << i << "] " << sp->Name() << "\n";
    }

    int idx; std::cin >> idx;
    if (idx < 0 || idx >= (int)player_->HandRef().Size())
        return;

    int tx, ty;
    std::cin >> tx >> ty;

    SpellContext ctx;
    if (!(tx == -1 && ty == -1))
        ctx.targetCell = {tx, ty};

    player_->HandRef().Use(idx, *player_, field_, ctx, true);
}

void Game::UpdateTurn() {
    player_->Update(field_);
    field_.UpdateAll();

    if (player_->HandRef().Size() < player_->HandRef().Capacity())
        player_->HandRef().Add(SpellFactory::MakeRandom());

    if (!player_->IsAlive())
        running_ = false;
   
    AdvanceState();
}

void Game::AdvanceState() {
    if (!player_->IsAlive()) {
        state_ = GameState::GameOver;
        running_ = false;
        return;
    }

    if (!field_.AnyEnemiesAlive()) {
        state_ = GameState::LevelCompleted;
        return;
    }

    state_ = GameState::Running;
}

void Game::StartNextLevel() {
    std::cout << "\n=== Level " << level_ << " cleared! Next level... ===\n";

    player_->OnLevelUp();
    ++level_;

    int newW = clampDim(field_.Width() + 2);
    int newH = clampDim(field_.Height() + 1);

    field_ = GameField(newW, newH);

    int px = newW / 2;
    int py = newH / 2;
    field_.PlacePlayer(player_, px, py);

    float wallDensity = 0.10f + 0.02f * (level_ - 1);
    if (wallDensity > 0.25f) wallDensity = 0.25f;
    field_.GenerateWalls(wallDensity);

    int enemies = 4 + level_ + std::rand() % 3;
    int baseHp  = 15 + 5 * (level_ - 1);
    int baseDmg = 4  + (level_ - 1);

    for (int i = 0; i < enemies; ++i) {
        int flag = 1;
        while (flag) {
            int ex = std::rand() % field_.Width();
            int ey = std::rand() % field_.Height();
            if (ex == px && ey == py) continue;
            auto& cell = field_.CellAt(ex, ey);
            if (!cell.isWalkable() || cell.hasEntity()) continue;
            cell.setEntity(std::make_shared<Enemy>("Enemy", baseHp, baseDmg));
            break;
        }
    }

    state_ = GameState::Running;
}
