#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Command.h"
#include "Game.h"
#include "GameExecutor.h"

// Шаблонный класс управления игрой 
template<typename InputHandlerType>
class GameManager {
private:
    InputHandlerType inputHandler;
    GameExecutor& executor;
    Game& game;
    bool running;
    
public:
    explicit GameManager(GameExecutor& exec, Game& g, InputHandlerType&& handler) 
        : inputHandler(std::move(handler)), executor(exec), game(g), running(true) {}
    
    bool process() {
        Command cmd = inputHandler.getCommand();
        
        if (!cmd.isValid()) {
            return false;
        }
        
        if (cmd.getType() == CmdType::QUIT_GAME) {
            running = false;
            return false;
        }
        
        bool action = executor.execute(cmd);
        
        if (action) {
            game.update();
            return true;
        }
        
        return false;
    }
    
    void stop() { running = false; }
    bool isRunning() const { return running; }
};

#endif