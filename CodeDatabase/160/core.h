#ifndef INCLUDE_CORE
#define INCLUDE_CORE

#include "game_board.h"

class Core {
private:
    int move_ticker_;

    void GenerateWalls(GameBoard& board, int wall_percentage);
    void GenerateFreezeTraps(GameBoard& board, int freeze_percentage);
    void GenerateEnemies(GameBoard& board);
    void GenerateEnemiesForLevel(GameBoard& board, int level);
    void GenerateTowers(GameBoard& board);
    void GenerateTowersForLevel(GameBoard& board, int level);
    void UpdateEnemies(GameBoard& board);
    void UpdateFriends(GameBoard& board);

public:
    Core();
    
    void InitializeBoard(GameBoard& board);
    void InitializeBoardForLevel(GameBoard& board, int level);  
    void UpdateGame(GameBoard& board);
};

#endif