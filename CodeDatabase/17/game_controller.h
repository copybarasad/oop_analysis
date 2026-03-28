#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include "game_command.h"
#include "position.h"

class Game;

template<typename InputHandler>
class GameController {
private:
    InputHandler inputHandler;
    Game* game; 
    bool flag = true;

public:
    GameController(Game* gameInstance = nullptr);
    ~GameController() = default;
    void setGame(Game* g);

    GameCommand getGameCommand();
    GameCommand getMenuCommand();
    GameCommand getSpellCommand();
    GameCommand getTargetCommand();

    Position processTargetSelection(Position cursor);
    bool processGameInput();
    void processMenu();

    const InputHandler* getInputHandlerPtr() const { 
        return &inputHandler; 
    }
    
};

#endif
