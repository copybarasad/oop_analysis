#ifndef SAVE_MANAGER_HPP
#define SAVE_MANAGER_HPP

#include <string>
#include <fstream>
#include <vector>

class Game;

class SaveManager {
private:
    std::string saveFileName;

public:
    SaveManager(const std::string& filename = "game_save.dat");
    bool saveGame(const Game& game);
    bool loadGame(Game& game);
    bool saveExists() const;
    void deleteSave();
};

#endif