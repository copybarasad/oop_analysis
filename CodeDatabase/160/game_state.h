#ifndef INCLUDE_CORE_GAME_STATE
#define INCLUDE_CORE_GAME_STATE

#include <algorithm>
#include <string>
#include <utility>

#include "core.h"
#include "game_board.h"
#include "../ui/shop.h"

struct GameState {
    GameBoard board;
    Core core;
    Shop shop;
    std::string saves_dir;

    explicit GameState(int rows, int cols, std::string savesDir = "saves");

    void NewGame();
    void RebuildBoardForLevel(int level);
};

#endif
