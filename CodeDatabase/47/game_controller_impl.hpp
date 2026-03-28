#ifndef GAME_CONTROLLER_IMPL_HPP
#define GAME_CONTROLLER_IMPL_HPP

#include <iostream>

template <typename InputReader, typename View>
GameController<InputReader, View>::GameController(Game& gameRef)
    : game(gameRef)
{
}

template <typename InputReader, typename View>
void GameController<InputReader, View>::run()
{
    while (!exitGame)
        showMainMenu();
}

template <typename InputReader, typename View>
void GameController<InputReader, View>::showMainMenu()
{
    returnToMenu = false;

    while (!exitGame && !returnToMenu)
    {
        std::cout << "\n=== MAIN MENU ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";

        int choice = getValidInput(1, 3);

        switch (choice)
        {
            case 1:
                game.startNewGame();
                mainLoop();
                break;

            case 2:
                if (game.loadGame())
                    mainLoop();
                else
                    std::cout << "Failed to load save.\n";
                break;

            case 3:
                exitGame = true;
                return;
        }
    }
}

template <typename InputReader, typename View>
void GameController<InputReader, View>::mainLoop()
{
    std::cout << "\n=== GAME STARTED ===" << std::endl;

    returnToMenu = false;

    while (!game.isGameOver() && !returnToMenu)
    {
        view.render(game);

        Command cmd = reader.readCommand();

        if (cmd.type == CommandType::Quit)
        {
            returnToMenu = true;
            return;
        }

        if (cmd.type == CommandType::Pause)
        {
            showPauseMenu();
            continue;
        }

        bool playerEndedTurn = game.applyCommand(cmd);

        if (playerEndedTurn && !game.isGameOver() && !game.isLevelCompleted())
        {
            game.enemyTurn();
            game.performPostEnemyChecks();
        }

        if (game.isLevelCompleted())
        {
            game.handleLevelCompletion();
            continue;
        }
    }

    if (game.isGameOver())
    {
        std::cout << "\n=== GAME OVER ===\n";
        std::cout << "Final Score: " << game.getTotalScore() << "\n";
        returnToMenu = true;
    }
}

template <typename InputReader, typename View>
void GameController<InputReader, View>::showPauseMenu()
{
    bool inPauseMenu = true;
    
    while (inPauseMenu)
    {
        std::cout << "\n=== PAUSE MENU ===\n";
        std::cout << "1. Resume\n";
        std::cout << "2. Save Game\n";
        std::cout << "3. Load Game\n";
        std::cout << "4. Quit to Main Menu\n";

        int choice = getValidInput(1, 4);

        switch (choice)
        {
            case 1:
                inPauseMenu = false;
                break;

            case 2:
                game.saveGame();
                break;

            case 3:
                game.loadGame();
                inPauseMenu = false;
                break;

            case 4:
                returnToMenu = true;
                inPauseMenu = false;
                break;
        }
    }
}

template <typename InputReader, typename View>
int GameController<InputReader, View>::getValidInput(int minChoice, int maxChoice)
{
    int x;
    bool validInput = false;
    
    while (!validInput)
    {
        std::cin >> x;

        if (!std::cin.fail() && x >= minChoice && x <= maxChoice)
        {
            std::cin.ignore(10000, '\n');
            validInput = true;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input! Try again.\n";
        }
    }
    
    return x;
}

#endif