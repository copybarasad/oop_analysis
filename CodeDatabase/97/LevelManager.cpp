#include "LevelManager.h"
#include "GameState.h"
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Spawner.h"

#include <algorithm>
#include <random>
#include <iostream>

int LevelManager::presetForLevel(int level) const { return (level - 1) % 3 + 1; }

void LevelManager::generateMapPreset(Field &field, int preset) {
    // Clear to empty
    for (int r = 0; r < field.rows(); ++r)
        for (int c = 0; c < field.cols(); ++c)
            field.setCellType({r,c}, CellType::Empty);

    // borders
    for (int c = 0; c < field.cols(); ++c) {
        field.setCellType({0,c}, CellType::Obstacle);
        field.setCellType({field.rows()-1,c}, CellType::Obstacle);
    }
    for (int r = 0; r < field.rows(); ++r) {
        field.setCellType({r,0}, CellType::Obstacle);
        field.setCellType({r,field.cols()-1}, CellType::Obstacle);
    }

    int R = field.rows(), C = field.cols();
    auto inb = [&](int r, int c){ return r>=0 && r<R && c>=0 && c<C; };

    if (preset == 1) {
        for (int i = 2; i < std::min(R-2, C-2); i += 3) {
            if (inb(i,i)) field.setCellType({i,i}, CellType::Obstacle);
            if (inb(i,i+1)) field.setCellType({i,i+1}, CellType::Obstacle);
        }
        if (inb(2,2)) field.setCellType({2,2}, CellType::Slow);
        if (inb(2,3)) field.setCellType({2,3}, CellType::Slow);
        if (inb(3,2)) field.setCellType({3,2}, CellType::Slow);
    } else if (preset == 2) {
        for (int r = 2; r < R-2; ++r) {
            if (r % 4 != 0) {
                int c = C/3; if (inb(r,c)) field.setCellType({r,c}, CellType::Obstacle);
                int c2 = 2*C/3; if (inb(r,c2)) field.setCellType({r,c2}, CellType::Obstacle);
            }
        }
        int mid = R/2;
        if (inb(mid, mid-1)) field.setCellType({mid, mid-1}, CellType::Slow);
        if (inb(mid, mid)) field.setCellType({mid, mid}, CellType::Slow);
        if (inb(mid, mid+1)) field.setCellType({mid, mid+1}, CellType::Slow);
    } else {
        for (int r = R-4; r <= R-2; ++r) for (int c = C-5; c <= C-2; ++c) if (inb(r,c)) field.setCellType({r,c}, CellType::Obstacle);
        for (int r = 1; r < R-1; ++r) if (r % 3 == 0 && inb(r,2)) field.setCellType({r,2}, CellType::Slow);
        int cr = R/2, cc = C/2; if (inb(cr, cc)) field.setCellType({cr, cc}, CellType::Slow);
        if (inb(cr+1, cc)) field.setCellType({cr+1, cc}, CellType::Slow);
    }
}

void LevelManager::generatePreset(GameState &state, int presetIndex) {
    generateMapPreset(state.model().field(), presetIndex);
}

void LevelManager::populateLevel(GameState &state) {
    Field &field = state.model().field();
    int preset = presetForLevel(state.level());
    generateMapPreset(field, preset);

    // helper
    auto placeIfFree = [&](std::shared_ptr<Entity> e, Coord pos)->bool{
        if (!field.inBounds(pos)) return false;
        if (!field.cellAt(pos).isPassable()) return false;
        if (field.cellAt(pos).hasEntity()) return false;
        field.placeEntity(std::move(e), pos);
        return true;
    };

    // Player: create only if missing
    if (!state.player()) {
        auto player = std::make_shared<Player>(25,7,4);
        state.setPlayer(player, Coord{2,6});
        player->hand().addSpell("DirectDamage");
        player->hand().addSpell("AreaDamage");
        player->hand().addSpell("Trap");
        player->hand().addSpell("Summon");
        player->hand().addSpell("Upgrade");
    } else {
        state.setPlayer(state.player(), Coord{2,6});
    }

    int enemyHp = 6 + (state.level() - 1) * 2;
    int enemyDmg = 2 + (state.level() - 1) / 2;

    if (preset == 1) {
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{3,6});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{5,2});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{9,9});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{10,6});
    } else if (preset == 2) {
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{3,4});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{6,2});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{8,10});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{11,6});
    } else {
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{4,7});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{6,3});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{9,11});
        placeIfFree(std::make_shared<Enemy>(enemyHp, enemyDmg), Coord{10,5});
    }

    // building and spawner placement (guaranteed)
    int buildingHp = 20 + (state.level() - 1) * 5;
    int spawnerHp = 10 + (state.level() - 1) * 2;

    bool placedB = false;
    Coord prefB{ std::max(1, field.rows()-3), std::max(1, field.cols()-3) };
    if (placeIfFree(std::make_shared<EnemyBuilding>(buildingHp,5), prefB)) placedB = true;
    else {
        for (int r = field.rows()-1; r >= 1 && !placedB; --r)
            for (int c = field.cols()-1; c >= 1 && !placedB; --c)
                if (!(r==2 && c==6) && placeIfFree(std::make_shared<EnemyBuilding>(buildingHp,5), Coord{r,c})) placedB = true;
    }
    if (!placedB) {
        for (int r=0;r<field.rows() && !placedB;++r)
            for (int c=0;c<field.cols() && !placedB;++c)
                if (field.cellAt({r,c}).isPassable() && !field.cellAt({r,c}).hasEntity()) {
                    field.placeEntity(std::make_shared<EnemyBuilding>(buildingHp,5), {r,c}); placedB=true; }
    }

    bool placedS = false;
    Coord cen{field.rows()/2, field.cols()/2};
    if (placeIfFree(std::make_shared<Spawner>(spawnerHp,3), cen)) placedS = true;
    else {
        int maxr = std::max(field.rows(), field.cols());
        for (int radius=1; radius<=maxr && !placedS; ++radius) {
            for (int dr=-radius; dr<=radius && !placedS; ++dr)
                for (int dc=-radius; dc<=radius && !placedS; ++dc) {
                    if (std::abs(dr)!=radius && std::abs(dc)!=radius) continue;
                    Coord pos{cen.row+dr, cen.col+dc}; if (!field.inBounds(pos)) continue;
                    if (placeIfFree(std::make_shared<Spawner>(spawnerHp,3), pos)) placedS = true;
                }
        }
    }
    if (!placedS) for (int r=0;r<field.rows() && !placedS;++r)
            for (int c=0;c<field.cols() && !placedS;++c)
                if (field.cellAt({r,c}).isPassable() && !field.cellAt({r,c}).hasEntity()) {
                    field.placeEntity(std::make_shared<Spawner>(spawnerHp,3), {r,c}); placedS=true;
                }

    // debug output
    std::optional<Coord> bcoord, scoord;
    for (int r=0;r<field.rows();++r) for (int c=0;c<field.cols();++c) {
            auto e = field.cellAt({r,c}).getEntity(); if (!e) continue;
            if (dynamic_cast<EnemyBuilding*>(e.get())) bcoord = Coord{r,c};
            if (dynamic_cast<Spawner*>(e.get())) scoord = Coord{r,c};
        }
    if (bcoord) std::cout << "(DBG) Building at ("<<bcoord->row<<","<<bcoord->col<<")\n";
    if (scoord) std::cout << "(DBG) Spawner at ("<<scoord->row<<","<<scoord->col<<")\n";
}
