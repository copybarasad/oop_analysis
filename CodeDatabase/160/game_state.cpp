#include "../include/core/game_state.h"

GameState::GameState(int rows, int cols, std::string savesDir)
    : board(rows, cols), core(), shop(), saves_dir(std::move(savesDir)) {}

void GameState::NewGame() {
    core.InitializeBoard(board);
}

void GameState::RebuildBoardForLevel(int level) {
    int new_rows = std::min(board.rows() + (level > 3 ? 1 : 0), 25);
    int new_cols = std::min(board.cols() + (level > 3 ? 1 : 0), 25);

    if (new_rows != board.rows() || new_cols != board.cols()) {
        GameBoard new_board(new_rows, new_cols);
        new_board.player() = board.player();
        core.InitializeBoardForLevel(new_board, level);
        board = std::move(new_board);
    } else {
        core.InitializeBoardForLevel(board, level);
    }
}
