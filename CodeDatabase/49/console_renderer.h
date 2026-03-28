#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "../game/game.h"
#include "../map/game_map.h"
#include "../player/player.h"
#include "../map/cell.h"
#include "../enemies/enemy_building.h"
#include "../enemies/enemy_tower.h"

class Game;
class GameMap;
class Cell;
class Player;
class SpellHand;

class ConsoleRenderer
{
private:
    const std::string COLOR_RESET = "\033[0m";
    const std::string COLOR_PLAYER = "\033[1;97m";
    const std::string COLOR_ENEMY = "\033[1;31m";
    const std::string COLOR_TREE = "\033[1;32m";
    const std::string COLOR_BUILDING = "\033[1;35m";
    const std::string COLOR_TOWER = "\033[1;34m";
    const std::string COLOR_TRAP = "\033[1;33m";
    const std::string COLOR_TRAP_SPELL = "\033[1;36m";
    const std::string COLOR_EMPTY = "\033[38;5;94m";
    const std::string COLOR_BORDER = "\033[1;36m";
    const std::string COLOR_COORDINATES = "\033[1;90m";

    void render_map(const GameMap& map) const;
    void render_legend() const;
    void render_player_stats(const Player* player) const;
    void render_spell_hand(const Player* player) const;
    void render_enemy_stats(const GameMap& map) const;

    struct CellVisual
    {
        char symbol;
        std::string color;
    };
    CellVisual get_cell_visual(const Cell& cell) const;

public:
    ConsoleRenderer() = default;

    void render(const Game& game) const;

    void clear_screen() const;
};

#endif