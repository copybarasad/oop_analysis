#include "console_renderer.h"

#include "enemy.h"
#include "enemy_tower.h"
#include "game.h"
#include "hand.h"
#include "player.h"
#include "point.h"

#include <iostream>
#include <string>

ConsoleRenderer::ConsoleRenderer() : out(&std::cout) {}
ConsoleRenderer::ConsoleRenderer(std::ostream& output) : out(&output) {}

namespace {

char CellSymbol(const Point& p) {
    if (!p.is_passable()) {
        return '#';
    }

    if (p.is_occupied()) {
        const Unit* u = p.get_occupier();
        if (dynamic_cast<const Player*>(u)) {
            return 'P';
        }
        if (dynamic_cast<const EnemyTower*>(u)) {
            return 'T';
        }
        if (dynamic_cast<const Enemy*>(u)) {
            return 'E';
        }
        return '?';
    }

    if (p.has_trap()) {
        return '^';
    }

    return '.';
}

void RenderGrid(std::ostream& out, const Grid& grid) {
    const int width = grid.get_width();
    const int height = grid.get_leng(); 

    out << "\n";
    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < height; ++x) {
            const Point* p = grid.getPoint(x, y);
            const char symbol = (p != nullptr) ? CellSymbol(*p) : '?';
            out << symbol << ' ';         
        }
        out << "\n";
    }
}

} 

void ConsoleRenderer::Render(const Game& game) {
    if (!out) {
        return;
    }

    std::ostream& o = *out;
    o << "\n\n";

    if (!game.last_message().empty()) {
        o << game.last_message() << "\n";
    }

    switch (game.state()) {
    case GameState::MAIN_MENU:
        o << "=== TRON GAME ===\n";
        o << "1) New game\n";
        o << "2) Load game\n";
        o << "3) Exit\n";
        o << "Choice: ";
        break;

    case GameState::PLAYING: {
        const Player* player = game.player();
        const Grid* grid = game.grid();

        o << "=== LEVEL " << game.level() << " ===\n";
        if (player) {
            o << "HP: " << player->get_hp() << "/" << player->get_max_hp()
              << " | Damage: " << player->get_damage()
              << " | Score: " << player->get_score()
              << " | Disk: " << (player->get_disk_type() == 0 ? "Melee" : "Range")
              << "\n";
        }

        if (player && player->get_hand()) {
            o << "Hacks:\n";
            const auto lines = player->get_hand()->ListSpells();
            if (lines.empty()) {
                o << "  (none)\n";
            } else {
                for (const auto& line : lines) {
                    o << "  " << line << "\n";
                }
            }
        }

        if (grid) {
            RenderGrid(o, *grid);
        }

        o << "\nControls:\n";
        o << "  w/a/s/d - move\n";
        o << "  f - switch weapon\n";
        o << "  c <spellIndex> <x> <y> - cast hack\n";
        o << "  i <y/n> - buy random hack for 10 score\n";
        o << "  v - save | l - load | q - back to menu\n";
        o << "Command: ";
        break;
    }

    case GameState::LEVEL_COMPLETE:
        o << "=== LEVEL COMPLETE ===\n";
        if (game.level() >= 3) {
            o << "Victory!\n";
            o << "1) Start new game\n";
            o << "2) Exit\n";
            o << "Choice: ";
        } else {
            o << "Choose upgrade:\n";
            o << "1) +20 HP\n";
            o << "2) +5 damage\n";
            o << "3) Skip\n";
            o << "Choice: ";
        }
        break;

    case GameState::GAME_OVER:
        o << "=== GAME OVER ===\n";
        o << "1) Restart from level 1\n";
        o << "2) Exit\n";
        o << "Choice: ";
        break;

    case GameState::EXIT:
        o << "Exiting...\n";
        break;
    }
}
