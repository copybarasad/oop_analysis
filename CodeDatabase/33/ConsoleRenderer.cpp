#include "ConsoleRenderer.h"
#include <iostream>
#include <vector>

void ConsoleRenderer::Render(const Game& game) const {
    std::vector<Coord> enemy_pos;
    std::vector<Coord> ally_pos;
    std::vector<Coord> tower_pos;
    std::vector<Coord> turret_pos;
    std::vector<Coord> trap_pos;

    for (const auto& e : game.Enemies()) {
        if (e.IsAlive()) {
            enemy_pos.push_back(e.Pos());
        }
    }
    for (const auto& a : game.Allies()) {
        if (a.IsAlive()) {
            ally_pos.push_back(a.Pos());
        }
    }
    for (const auto& t : game.Towers()) {
        tower_pos.push_back(t.Pos());
    }
    for (const auto& r : game.Turrets()) {
        if (r.IsAlive()) {
            turret_pos.push_back(r.Pos());
        }
    }
    for (const auto& tr : game.Traps()) {
        trap_pos.push_back(tr.Pos());
    }

    game.GetField().Render(
        game.GetPlayer().Pos(),
        enemy_pos,
        ally_pos,
        tower_pos,
        turret_pos,
        trap_pos
    );

    const auto& p = game.GetPlayer();
    std::cout
        << "Name: " << p.Name()
        << "  HP: " << p.HP()
        << "  Mode: " << (p.CurrentMode()==Player::Mode::Melee ? "MELEE":"RANGED")
        << "  Dmg: " << p.CurrentDamage()
        << "  Score: " << p.Score()
        << "  Enemies: " << game.Enemies().size()
        << "  Allies: " << game.Allies().size()
        << "  Towers: " << game.Towers().size()
        << "  Turrets: " << game.Turrets().size()
        << "  Hand: " << game.GetHand().Size() << "/" << game.GetHand().Capacity()
        << (game.MoveLockedNextTurn() ? "  [SLOWED]" : "")
        << "\n";

    game.GetHand().PrintHand();
}
