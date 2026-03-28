#ifndef GAMELOOP_CPP
#define GAMELOOP_CPP

#include "GameLoop.h"
#include <iostream>

template<typename InputReaderType, typename RendererType>
bool GameLoop<InputReaderType, RendererType>::run() {
    view->showWelcome();
    
    while (game->isRunning()) {
        // Обработка одного хода
        if (!processTurn()) {
            return false; // Выход в главное меню
        }
        
        // Проверка поражения
        if (!game->getPlayer()->isAlive()) {
            if (!handleDefeat()) {
                return false; // Выход в меню
            }
            // продолжаем цикл while
        }
        
        // Проверка победы (все враги и башни уничтожены)
        if (game->getEnemies().empty() && game->getTowers().empty()) {
            if (!handleVictory()) {
                return false; // Выход в меню
            }
            // После победы и перехода на новый уровень продолжаем цикл
        }
    }
    
    return true;
}

template<typename InputReaderType, typename RendererType>
bool GameLoop<InputReaderType, RendererType>::processTurn() {
    // Обработать ввод
    bool continueGame = controller->processInput();
    
    // Рендерить только если нужно (не было ошибки)
    if (controller->shouldRender()) {
        view->render();
    }
    
    return continueGame;
}

template<typename InputReaderType, typename RendererType>
bool GameLoop<InputReaderType, RendererType>::handleDefeat() {
    view->showGameOver();
    
    // Показать меню поражения
    auto choice = MenuSystem::showDefeatMenu();
    
    if (choice == MenuSystem::DefeatMenuChoice::NEW_GAME) {
        std::cout << "\nStarting new game...\n";
        
        game->startNewGame();  // Это должно быть в классе Game
        view->showWelcome();
        
        return true;  // Продолжить текущий игровой цикл
    }
    
    std::cout << "\nReturning to main menu...\n";
    return false;
}

template<typename InputReaderType, typename RendererType>
bool GameLoop<InputReaderType, RendererType>::handleVictory() {
    view->showVictory();
    
    // Показать меню улучшений
    const Upgrade* selectedUpgrade = MenuSystem::showUpgradeMenu(game->getUpgradeSystem());
    
    if (selectedUpgrade) {
        game->applyUpgrade(selectedUpgrade);
        std::cout << "\nUpgrade applied: " << selectedUpgrade->name << "\n";
    }
    
    //  хочет ли игрок перейти на следующий уровень
    if (MenuSystem::askNextLevel()) {
        game->nextLevel();
        std::cout << "\n=============================\n";
        std::cout << "     LEVEL " << game->getCurrentLevel() << "\n";
        std::cout << "=============================\n\n";
        return true; // Продолжить игру
    }
    
    std::cout << "\nReturning to main menu...\n";
    return false;
}

#endif // GAMELOOP_CPP


