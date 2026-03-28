#pragma once
#include <string>

class Game; 

class GameSaver {
public:
    static void saveGame(const Game& game, const std::string& filename);
    static void loadGame(Game& game, const std::string& filename);
};