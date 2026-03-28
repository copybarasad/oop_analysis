#pragma once
#include <string>
#include <fstream>
#include "GameExceptions.h"

class Game;

class SaveManager {
public:
    static void saveGame(const Game& game, const std::string& filename);
    static void loadGame(Game& game, const std::string& filename);
};
