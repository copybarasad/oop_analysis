#pragma once
#include "GameState.h"
#include "FileHandler.h"
#include "SaveLoadException.h"
#include <sstream>

class SaveManager {
public:
    static void save_game(GameState& state, const std::string& filename = "save.txt");
    static GameState load_game(const std::string& filename = "save.txt");
};
