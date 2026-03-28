#include "GameController.h"
#include "ConsoleInputAdapter.h"
#include "ConsoleRenderAdapter.h"
#include "GameSaver.h"
#include "GameLoader.h"
#include <iostream>
#include <Windows.h>

void showMainMenu() {
    std::cout << "\n=== MAGIC TOWER GAME ===\n";
    std::cout << "1. Новая игра\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Показать сохранения\n";
    std::cout << "4. Выйти\n";
    std::cout << "Выберите вариант: ";
}
void startNewGame() { 
    GameField field(15, 15);
    Player player(3);
    Enemy enemy(1);
    EnemyManager enemyManager;
    LevelManager levelManager;
    GameStateManager stateManager;
     
    auto inputAdapter = std::make_unique<ConsoleInputAdapter>();
    auto renderAdapter = std::make_unique<ConsoleRenderAdapter>();
     
    auto restartCallback = [&field, &player, &enemy, &enemyManager, &levelManager]() {
        field.reset(15, 15);
        player = Player(3);
        player.getSpellHand().addRandomSpell();
        enemy = Enemy(1);
        enemyManager.reset();
        enemyManager.setLevel(1);
        levelManager.setLevel(1);
    };
     
    GameController<ConsoleInputAdapter, ConsoleRenderAdapter> game(
        field, player, enemy, enemyManager, 
        levelManager, stateManager,
        std::move(inputAdapter), 
        std::move(renderAdapter),
        restartCallback   
    );
    
    game.run();
}

void loadGame() {
    GameSaver::showSaves();
    std::cout << "\nВыберите слот для загрузки (1-5): ";
    int slot;
    std::cin >> slot;
    while (std::cin.get() != '\n') continue;
    
    if (slot >= 1 && slot <= 5) {
        GameField field(15, 15);
        Player player(3);
        Enemy enemy(1);
        EnemyManager enemyManager;
        LevelManager levelManager;
        GameStateManager stateManager;
        
        auto inputAdapter = std::make_unique<ConsoleInputAdapter>();
        auto renderAdapter = std::make_unique<ConsoleRenderAdapter>();
         
        auto restartCallback = [&field, &player, &enemy, &enemyManager, &levelManager]() {
            field.reset(15, 15);
            player = Player(3);
            player.getSpellHand().addRandomSpell();
            enemy = Enemy(1);
            enemyManager.reset();
            enemyManager.setLevel(1);
            levelManager.setLevel(1);
        };
        
        GameController<ConsoleInputAdapter, ConsoleRenderAdapter> game(
            field, player, enemy, enemyManager, 
            levelManager, stateManager,
            std::move(inputAdapter), 
            std::move(renderAdapter),
            restartCallback   
        );
        
        game.setLoadingFromSave(true);
        
        if (GameLoader::loadGame(slot, field, player, enemy, enemyManager, levelManager)) {
            game.run();
        }
    } else {
        std::cout << "Неверный номер слота!\n";
    }
}
int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    int choice;
    
    do {
        showMainMenu();
        std::cin >> choice;
        while (std::cin.get() != '\n') continue;
        
        switch (choice) {
            case 1:
                startNewGame();
                break;
            case 2:
                loadGame();
                break;
            case 3:
                GameSaver::showSaves();
                break;
            case 4:
                std::cout << "Выход из игры...\n";
                break;
            default:
                std::cout << "Неверный выбор!\n";
                break;
        }
    } while (choice != 4);
    
    return 0;
}