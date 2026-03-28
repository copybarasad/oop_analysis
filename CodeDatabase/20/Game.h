#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <sstream>

#include "Gamefield.h"
#include "Enemy.h"
#include "Player.h"
#include "Cell.h"
#include "Tower.h"
#include "Command.h"

struct SaveData {
    int width, height;
    int px, py, php;

    struct EnemyData { int x,y,hp,dmg; };
    struct TowerData { int x,y,hp,dmg,range; };

    std::vector<EnemyData> enemies;
    std::vector<TowerData> towers;
};

class Game {
private:
    // States
    enum class TurnState { PLAYER, ENEMY, EN_TOWER };
    enum class GameState { MENU, IN_GAME, PAUSED, GAME_OVER, LEVEL_COMPLETE };

    // Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    // Game objects
    Gamefield gamefield;
    const int cellSize = CELL_SIZE;

    GameState state;
    TurnState turn;

    Player player;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<TeslaTower>> towers;

    bool playerDeathHandled = false;


    // Level management
    int currentLevel = 1;    
    int roundCounter = 0;

    // Initialization
    void initVariables();
    void initWindow();
    void initEntities(int level);   

    // Turn handlers
    void updateTurn();
    void handleEnemies();
    void handleTowers();
    void handlePlayerDeath();
    void cleanup();

    // State checks
    bool checkVictory();          

    // Victory and next level
    void handleLevelComplete();    
    void nextLevel();

    // States and screens
    void showMainMenu();
    void handleGameOver();
    void startNewGame();

    // Saving
    uint32_t calculateChecksum(std::ostringstream& ss);
    void SaveGame(const std::string& filename);
    void LoadGame(const std::string& filename);
    

    // Helpers
    bool isAdjacent(int ax, int ay, int bx, int by);

public:
    // Constructors
    Game();
    Game(int width, int length);
    ~Game();

    // Main loop
    const bool running() const;
    void update();
    void render();
    void PollEvents();

};
