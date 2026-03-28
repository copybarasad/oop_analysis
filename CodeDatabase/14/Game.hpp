#ifndef GAME_H
#define GAME_H

#include "GameBoard.hpp"
#include "observer.hpp"
#include <string>

class Game {
private:
    GameBoard* board;
    int current_level;
    bool is_running;
    
    double enemy_hp_multiplier;
    int board_width;
    int board_height;

    void nextLevel();
    void cleanUp();
    void showUpgradeMenu(); 

public:
    Game();
    ~Game();

    void initLevel(bool new_game);
    void attachLogger(IObserver* observer);

    void movePlayer(int dx, int dy);
    void playerAttack();
    void switchPlayerWeapon();
    void castSpell(int spell_index, int targetX, int targetY);

    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    void stop(); 

    bool isRunning() const { return is_running; }
    const GameBoard* getBoard() const { return board; }
    
    void updateWorld();
};

#endif