#include "StartGame.h"
#include <iostream>
#include <cstdlib>  

bool GameStarter::ValidateFieldSize(int width, int height) {
    return (width >= 10 && width <= 25 && height >= 10 && height <= 25);
}

bool GameStarter::ValidateChance(double chance) {
    return (chance >= 0.0 && chance <= 1.0);
}

bool GameStarter::ValidateEnemyCount(int count) {
    return (count > 0);
}

void GameStarter::ShowMainMenu() {
    std::cout << "\n========== ЗАПУСК ИГРЫ. ПРИВЕТ ==========" << std::endl;
    std::cout << "1 - Новая игра" << std::endl;
    std::cout << "2 - Загрузить сохранение" << std::endl;
    std::cout << "0 - Выйти" << std::endl;
    std::cout << "Ваш выбор: ";
}

void GameStarter::StartNewGame() {
    int w, h, enemies;
    double chance;

    std::cout << "\n=== СОЗДАНИЕ НОВОЙ ИГРЫ ===" << std::endl;
    
    std::cout << "Введи ширину и высоту поля [10;10] - [25;25]: ";
    std::cin >> w >> h;
    if(!ValidateFieldSize(w, h)){
        std::cout << "Размеры поля должны быть в диапазоне [10;10] - [25;25]" << std::endl;
        exit(0);  
    }

    std::cout << "Введи шанс появления препятствий [0.0–1.0]: ";
    std::cin >> chance;
    if(!ValidateChance(chance)){
        std::cout << "Шанс должен быть в пределах [0.0–1.0]" << std::endl;
        exit(0);  
    }

    std::cout << "Введи количество врагов: ";
    std::cin >> enemies;
    if(!ValidateEnemyCount(enemies)){
        std::cout << "Количество врагов должно быть больше 0" << std::endl;
        exit(0);  
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // почистить буфер ввода 
    Game game(w, h, chance, enemies);
    
    RunGame<UserInputConsole, ConsoleRenderer>(game);
}


bool GameStarter::LoadExistingGame() {
    std::cout << "\n=== ЗАГРУЗКА СОХРАНЕНИЯ ===" << std::endl;
    
    GameState state;
    
    if (StoreGame::LoadGame(state, "savegame.txt")) {
        int width = state.GetFieldWidth();
        int height = state.GetFieldHeight();
        double chance = state.GetBlockChance();
        int enemyCount = state.GetBaseEnemyCount();
        
        Game tempGame(width, height, chance, enemyCount);
        
        tempGame.RestoreFromState(state);
        
        RunGame<UserInputConsole, ConsoleRenderer>(tempGame);
        return true;
    }
    
    std::cout << "Не удалось загрузить сохранение." << std::endl;
    return false;
}


template<typename InputT, typename RenderT>
void GameStarter::RunGame(Game& game) {
    GameController<InputT, RenderT> controller;
    
    game.SetLevelActive(true);
    
    std::cout << "\n=== ИГРА НАЧАЛАСЬ! ===" << std::endl;
    std::cout << "Используйте буквенные команды (w, a, s, d, f и т.д.)" << std::endl;
    
    while (game.IsLevelActive() && game.PlayerIsAlive() && !game.IsLevelCompleted()) {
        controller.Run(game);
    }
    
    if (!game.PlayerIsAlive()) {
        std::cout << "Вы мертвы. Попробовать снова? [1 — да, 0 — нет]: ";
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            game.GetPlayer().RestoreHealth();
        }
    } else if (game.IsLevelCompleted()) {
        std::cout << "Вы победили всех врагов на уровне " << game.GetCurrentLevel() << std::endl;
        game.NextLevel();
    }
}


void GameStarter::Run() {
    bool running = true;
    
    while (running) {
        ShowMainMenu();
        
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                StartNewGame();
                break;
                
            case 2:
                if (!LoadExistingGame()) {
                    std::cout << "Возврат в главное меню." << std::endl;
                }
                break;
                
            case 0:
                std::cout << "Выход из игры. Ну поке" << std::endl;
                running = false;
                break;
                
            default:
                std::cout << "Неверный выбор. Давай еще разок" << std::endl;
                break;
        }
    }
}