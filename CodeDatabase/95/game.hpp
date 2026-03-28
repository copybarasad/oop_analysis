#ifndef GAME_HPP
#define GAME_HPP

#include "game_state.hpp"
#include "level_manager.hpp"
#include "sys/input_handler.hpp"
#include "render/renderer.hpp"
#include "render/cli_render.hpp"

class Game {
private:
    GameState gameState;
    LevelManager levelManager; 
    Renderer<CLIRenderer> gameRenderer;
    
    void processPlayerTurn();
    void processEnemiesTurn();
    void processTowerTurn();
    void checkGameState();
    void processCommand(GameCommand& cmd);
    
public:
    Game(int startLevel = 1);
    
    GameStatus run(TerminalInputHandler& input);
    bool proceedToNextLevel();
    
    GameState& getGameState();
    const GameState& getGameState() const;
    bool isRunning() const;

};

#endif