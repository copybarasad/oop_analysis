#include "game.hpp"
#include "level.hpp"
#include <iostream>
#include "game.hpp"
#include "level.hpp"
#include "game_framework.hpp"  

Game::Game() : currentLevel(0), gameRunning(true), saveManager("game_save.dat")
{
    initializeLevels();
}

Game::~Game()
{
    saveManager.deleteSave();
    std::cout << "Save file cleaned up." << std::endl;
}

const Level &Game::getLevel(int index) const
{
    return *levels[index];
}

Level &Game::getLevel(int index)
{
    return *levels[index];
}

void Game::run()
{
    //новое
    ConsoleInputReader input;
    ConsoleRenderer renderer;
    GameVisualizer<ConsoleRenderer> visualizer(renderer);
    GameController<ConsoleInputReader> controller;
    controller.runMainMenu(*this, visualizer, input);
}

void Game::continueLoadedGame()
{
    bool gameWon = true;
    for (size_t i = currentLevel; i < levels.size() && gameWon; ++i)
    {
        currentLevel = i;

        std::cout << "\n=== LEVEL " << (i + 1) << " ===" << std::endl;
        bool levelResult = levels[i]->run();

        if (levelResult)
        {
            std::cout << "Level " << (i + 1) << " completed!" << std::endl;
            if (i < levels.size() - 1)
            {
                saveGame();
            }
        }
        else
        {
            if (levels[i]->isFailed())
            {
                gameWon = false;
                showGameOverMenu(false);
            }
            else
            {
                return;
            }
        }
    }

    if (gameWon)
    {
        showGameOverMenu(true);
        saveManager.deleteSave();
    }
}

void Game::startNewGame()
{
    currentLevel = 0;
    bool gameWon = true;

    initializeLevels();
    saveManager.deleteSave();

    saveGame();

    for (size_t i = 0; i < levels.size() && gameWon; ++i)
    {
        currentLevel = i;
        levels[i]->reset();
        std::cout << "\n=== LEVEL " << (i + 1) << " ===" << std::endl;
        bool levelResult = levels[i]->run();
        if (levelResult)
        {
            // удаляем половину
            if (i < levels.size() - 1)
            {
                Hand &hand = levels[i]->getSpellHand();
                int spellCount = hand.get_spellCount();
                if (spellCount > 1)
                {
                    int spellsToRemove = spellCount / 2;
                    std::cout << "Removing " << spellsToRemove << " spell(s) for balance..." << std::endl;
                    for (int j = 0; j < spellsToRemove; j++)
                    {
                        hand.removeRandomSpell();
                    }
                }
                saveGame();
            }
        }
        else
        {
            if (levels[i]->isFailed())
            {
                gameWon = false;
                showGameOverMenu(false);
            }
            else
            {
                return;
            }
        }
    }

    if (gameWon)
    {
        showGameOverMenu(true);
        saveManager.deleteSave();
    }
}

bool Game::runLevel(int levelIndex)
{
    return levels[levelIndex]->run();
}

bool Game::loadGame()
{
    try
    {
        if (!saveManager.saveExists())
        {
            std::cout << "No save file found!" << std::endl;
            std::cout << "Start a new game to create a save file." << std::endl;
            return false;
        }

        if (saveManager.loadGame(*this))
        {
            std::cout << "\nStarting Level " << (currentLevel + 1) << "..." << std::endl;
            if (currentLevel >= levels.size())
            {
                throw std::runtime_error("Loaded level does not exist");
            }
            levels[currentLevel]->reset();
            bool result = levels[currentLevel]->run();
            if (result)
            {
                if (currentLevel < levels.size() - 1)
                {
                    saveGame();
                }
            }
            return true;
        }
        return false;
    }
    catch (const std::exception &e)
    {
        std::cout << "Critical error during game loading: " << e.what() << std::endl;
        std::cout << "Please start a new game." << std::endl;
        return false;
    }
}

void Game::saveGame()
{
    try
    {
        if (!saveManager.saveGame(*this))
        {
            std::cout << "Warning: Game progress was not saved!" << std::endl;
            std::cout << "Your progress will be lost if you quit." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "Unexpected error during save: " << e.what() << std::endl;
    }
}

void Game::showMainMenu()
{
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;

    if (saveManager.saveExists())
    {
        std::cout << "(Save file detected)" << std::endl;
    }

    std::cout << "Choice: ";

    std::string choice;
    std::cin >> choice;

    if (choice == "1")
    {
        startNewGame();
    }
    else if (choice == "2")
    {
        if (loadGame())
        {
            continueLoadedGame();
        }
    }
    else if (choice == "3")
    {
        gameRunning = false;
    }
    else
    {
        std::cout << "Invalid choice! Please enter 1, 2 or 3." << std::endl;
    }
}

void Game::showGameOverMenu(bool won)
{
    //новое
    ConsoleInputReader input;
    ConsoleRenderer renderer;
    GameVisualizer<ConsoleRenderer> visualizer(renderer);

    visualizer.renderGameOver(won);
    GameOverCommand cmd = input.readGameOverCommand();

    switch (cmd.type)
    {
    case GameOverCommandType::RestartGame:
        startNewGame();
        break;
    case GameOverCommandType::BackToMainMenu:
        // просто вернуться в главное меню
        break;
    case GameOverCommandType::Exit:
        gameRunning = false;
        break;
    case GameOverCommandType::Invalid:
    default:
        std::cout << "Invalid choice!" << std::endl;
        break;
    }
}

void Game::initializeLevels()
{
    levels.clear();
    levels.push_back(std::make_unique<Level>(this, 1, 10, 10));
    levels.push_back(std::make_unique<Level>(this, 2, 12, 12));
    levels.push_back(std::make_unique<Level>(this, 3, 15, 15));
}
