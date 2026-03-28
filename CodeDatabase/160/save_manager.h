#ifndef INCLUDE_SAVE_MANAGER
#define INCLUDE_SAVE_MANAGER

#include <string>
#include <vector>
#include "game_board.h"

class SaveManager {
public:
    static std::vector<std::string> ListSaves(const std::string& dir);
    static bool Save(const std::string& dir, const std::string& name, const GameBoard& board);
    static bool Load(const std::string& dir, const std::string& name, GameBoard& board);
private:
    static std::string CellTypeToStr(int v);
    static int StrToCellType(const std::string& s);
};

#endif