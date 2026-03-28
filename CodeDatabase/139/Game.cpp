#include "Game.h"
#include "States/MenuState.h"
#include <iostream>


Game::Game(int width, int height, int cellSize) 
{ 
    if (cellSize < 10)
        cellSize = 10;

    if (cellSize > 30)
        cellSize = 30;

    this->cellSize = cellSize;
    executeUseCase = new ExecuteUseCase(width, height, cellSize);
    currentState = new MenuState(this);
}


ExecuteUseCase* Game::getExecuteUseCase() { return executeUseCase; }


GameState* Game::getCurrentState() { return currentState; }


void Game::run() { isRunning = true; }

bool Game::running() const { return isRunning; }

void Game::stop() { isRunning = false; }


void Game::runActiveLevel() { isActiveLevel = true; }

void Game::stopActiveLevel() { isActiveLevel = false; }

bool Game::hasActiveLevel() const { return isActiveLevel; }


void Game::setState(GameState* newState)
{
    if (currentState) {
        delete currentState;
    }
    currentState = newState;
}


void Game::saveGame() const { executeUseCase->saveGame(); }

bool Game::loadGame()
{

    if (executeUseCase->loadGame(this->gameLevel))
    {
        isActiveLevel = true;
        const auto* field = executeUseCase->getGameBoard()->getField();
        requestWindowResize(field->getWidth(true) + rightExpansion, field->getHeight(true));
        return true;
    }
    return false;
}


void Game::nextLevel(int addHealth, int addDamage)
{
    isActiveLevel = true;
    gameLevel = gameLevel + 1 <= gameLevelMax ? gameLevel + 1 : gameLevelMax;
    Point newSize = executeUseCase->newGame(gameLevel, addHealth, addDamage);

    requestWindowResize(newSize.X() + rightExpansion, newSize.Y());
}

void Game::resetLevel()
{
    gameLevel = 1;
    isActiveLevel = true;
    Point newSize = executeUseCase->newGame(gameLevel);
    requestWindowResize(newSize.X() + rightExpansion, newSize.Y());
}

void Game::continueLevel()
{
    const auto* field = executeUseCase->getGameBoard()->getField();
    requestWindowResize(field->getWidth(true) + rightExpansion, field->getHeight(true));
}


Point Game::getSize() const { return Point(width, height); }


void Game::requestThemeChange(GameTheme theme) 
{
    requestedTheme = theme;
    themeChangeRequested = true;
}

bool Game::isThemeChangeRequested() const { return themeChangeRequested; }

void Game::clearThemeChangeRequest() { themeChangeRequested = false; }

GameTheme Game::getRequestedTheme() const { return requestedTheme; }


void Game::requestUpdateGame() { gameUpdateRequested = true; }

bool Game::isUpdateGameRequested() const { return gameUpdateRequested; }

void Game::clearUpdateGameRequest() { gameUpdateRequested = false; }


unsigned short Game::getGameLevelMax() const { return gameLevelMax; }

unsigned short Game::getGameLevel() const { return gameLevel; }


unsigned int Game::fieldWidth() const { return executeUseCase->getGameBoard()->getField()->getWidth(true); }

unsigned int Game::fieldHeight() const { return executeUseCase->getGameBoard()->getField()->getHeight(true); }


void Game::requestWindowResize(int w, int h) 
{
    desiredWidth = w;
    desiredHeight = h;
    windowResizeRequested = true;
}

bool Game::isResizeRequested() const { return windowResizeRequested; }
void Game::clearResizeRequest() { windowResizeRequested = false; }

int Game::getDesiredWidth() const { return desiredWidth; }
int Game::getDesiredHeight() const { return desiredHeight; }


int Game::getBuffHealth() const { return newLevelBuffHealth; }

int Game::getBuffDamage() const { return newLevelBuffDamage; }


const std::string& Game::getKeyConfigPath() const { return keyConfigPath; }


Game::~Game()
{
    if (executeUseCase != nullptr)
    {
        delete executeUseCase;
        executeUseCase = nullptr;
    }

    if (currentState != nullptr)
    {
        delete currentState;
        currentState = nullptr;
    }
}