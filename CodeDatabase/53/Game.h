#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <limits>
#include <memory>
#include "GameField.h"
#include "Player.h"
#include "Input.h"
#include "HandSpell.h"
#include "GameSaveManager.h"

using namespace std;

template<typename T>
class GameRenderer;

template<typename T>
class GameController;


class OutputTerminal;

class Game {
private:
    
    GameField field;
    Player player;
    GameSaveManager saveManager;
    Input inputHandler;
    
    int turn;
    int currentLevel;
    bool gameRunning;
    
    std::unique_ptr<GameRenderer<OutputTerminal>> gameRendererPtr;
    std::unique_ptr<GameController<Input>> gameControlPtr;
    void firstLevel();
    void secondLevel();
    void thirdLevel();
    
public:
    Game();
    ~Game(); 
    
    void initializeLevel(int level);
    
    int getCurrentLevel() const;
    int getCurrentTurn() const;
    const Player& getPlayer() const;
    const GameField& getField() const;
    int getTurn() const;
    bool isRunning() const;

    void setCurrentTurn(int newTurn);
    
    Player& getPlayerRef();
    GameField& getFieldRef();
    GameSaveManager& getSaveManagerRef();
    Input& getInputHandlerRef();
    int& getTurnRef();
    int& getCurrentLevelRef();
    bool& getGameRunningRef();
    GameController<Input>& getGameControl();
    GameRenderer<OutputTerminal>& getGameRender();
};

#endif