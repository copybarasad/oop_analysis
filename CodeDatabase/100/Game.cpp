#include "Game.h"
#include "../commands/MoveCommand.h"
#include "../commands/SwitchCombatModeCommand.h"
#include "../commands/CastSpellCommand.h"
#include "../commands/PauseCommand.h"
#include "../commands/GameController.h"
#include "../interface/GameVisualizer.h" 
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

Game::Game(int fieldWidth, int fieldHeight, int playerHealth, int playerDamage) :
                state(fieldWidth, fieldHeight, playerHealth, playerDamage), combat(state) { 

    std::random_device rd;
    rng = std::mt19937(rd());

    initializeGame();
}

Game::~Game() { }

void Game::initializeGame() {
    int playerX, playerY;
    bool positionFound = false;
    
    std::uniform_int_distribution<int> distX(0, state.getField().getWidth() - 1);
    std::uniform_int_distribution<int> distY(0, state.getField().getHeight() - 1);

    // Пытаемся найти подходящую позицию  
    for (int attempts = 0; attempts < 50; attempts++) {
        playerX = distX(rng);
        playerY = distY(rng);
        
        // Проверяем что клетка проходимая и не замедляющая
        if (state.getField().isCellPassable(playerX, playerY) && !state.getField().isCellSlowing(playerX, playerY)) {
            positionFound = true;
            break;
        }
    }

    if (!positionFound) {
        throw std::runtime_error("Failed to find suitable position for player");
    }
    state.getPlayer().setPosition(playerX, playerY);

    // Даём рандомное заклинание
    std::uniform_int_distribution<int> spellDist(0, 4);
    int spellType = spellDist(rng);

    std::shared_ptr<Spell> newSpell;
    if (spellType == 0) {
        newSpell = std::make_shared<DirectDamageSpell>(15, 3);
    } else if (spellType == 1) {
        newSpell = std::make_shared<AreaDamageSpell>(10, 4, 2);
    } else if (spellType == 2) {
        newSpell = std::make_shared<TrapSpell>(25, 4);
    } else if (spellType == 3) {
        newSpell = std::make_shared<SpawnAllySpell>(1, 60, 10, 'a');
    } else if (spellType == 4) {
        newSpell = std::make_shared<UpgradeSpell>(3);
    }
    state.addSpellToHand(newSpell);
    // Инциализируем всех других сущностей
    initializeCharacters(); 
}

// Инциализация сущностей
void Game::initializeCharacters() {
    state.addCharacter(&state.getPlayer());

    int enemyCount = 1; // Количество активных врагов при запуске игры
    int added = 0;
    int attempts = 0;

    int width = state.getField().getWidth();
    int height = state.getField().getHeight();

    std::uniform_int_distribution<int> distX(0, width - 1);
    std::uniform_int_distribution<int> distY(0, height - 1);

    while (added < enemyCount && attempts < 50) {
        int x = distX(rng);
        int y = distY(rng);

        if (!state.getField().isCellPassable(x, y) || state.getField().isCellSlowing(x, y))
            continue;

        // Проверка не занята ли клетка другим персонажем
        bool occupied = false;
         for (const auto& character : state.getAllCharacters()) {
            if (character->getX() == x && character->getY() == y) {
                occupied = true;
                break;
            }
        }
        if (occupied)
            continue;

        // Доабвляем нового врага
        state.addCharacter(new Enemy(30, 15, x, y, 'E'));
        added++;
    } 
    if (added < enemyCount) {
        throw std::runtime_error("Failed to find suitable position for enemy");
    }

    // Спавним вражеское здание и башню
    spawnEnemyBuilding();
    spawnEnemyTower();
}

void Game::spawnEnemyTower() {
    int width = state.getField().getWidth();
    int height = state.getField().getHeight();

    std::uniform_int_distribution<int> distX(0, width - 1);
    std::uniform_int_distribution<int> distY(0, height - 1);

    int towerX, towerY;
    bool posFound = false;

    for (int attempts = 0; attempts < 50; attempts++) {
        towerX = distX(rng);
        towerY = distY(rng);

        if (state.getField().isCellPassable(towerX, towerY) &&
            !state.getField().isCellSlowing(towerX, towerY)) {
            bool occupied = false;
            for (const auto& character : state.getAllCharacters()) {
            if (character->getX() == towerX && character->getY() == towerY) {
                    occupied = true;
                    break;
                }
            }
            for (const auto& building : state.getAllBuildings()) {
                if (building->getX() == towerX && building->getY() == towerY) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                posFound = true;
                break;
            }
        }
    }
    if (!posFound) {
        throw std::runtime_error("Failed to find suitable position for tower");
    }

    EnemyTower* tower = new EnemyTower(60, towerX, towerY, 'T');
    state.addTower(tower);

}

void Game::spawnEnemyBuilding() {
    int width = state.getField().getWidth();
    int height = state.getField().getHeight();

    std::uniform_int_distribution<int> distX(0, width - 1);
    std::uniform_int_distribution<int> distY(0, height - 1);

    int buildX, buildY;
    bool posFound = false;

    // Пытаемся поставить вражеское здание
    for (int attempts = 0; attempts < 50; attempts++) {
        buildX = distX(rng);
        buildY = distY(rng);

        if (!state.getField().isCellPassable(buildX, buildY)) continue;

        // Првоерка не занята ли клетка другими персонажами
        bool occupied = false;
        for (const auto& character : state.getAllCharacters()) {
            if (character->getX() == buildX && character->getY() == buildY) {
                occupied = true;
                break;
            }
        }

        // Спавним вражеское здание
        if (!occupied) {
            posFound = true;
            break;
        }
    }

    if (!posFound) {
        throw std::runtime_error("Failed to find suitable position for enemy building");
    }
;
    EnemyBuilding* building = new EnemyBuilding(100, buildX, buildY, 10, 'B');
    state.addBuilding(building);
}

// Выдать рандомное заклинание
void Game::tryGiveRandomSpell() {
    // Проверяем условия: прошло достаточно ходов и есть место в руке
    if (state.shouldGiveSpell()) {

        std::uniform_int_distribution<int> spellDist(0, 4);
        int spellCounter = spellDist(rng);
        std::shared_ptr<Spell> newSpell;
        switch (spellCounter) {
            case 0:
                newSpell = std::make_shared<DirectDamageSpell>(15, 3);
                break;
            case 1:
                newSpell = std::make_shared<AreaDamageSpell>(10, 4, 2);
                break;
            case 2:
                newSpell = std::make_shared<TrapSpell>(25, 4);
                break;
            case 3:
                newSpell = std::make_shared<SpawnAllySpell>(1, 60, 10, 'a');
                break;
            case 4:
                newSpell = std::make_shared<UpgradeSpell>(3);
                break;
        }

        if (state.addSpellToHand(newSpell)) {
            state.addMessage(Colors::GREEN + "You received a new spell: " + 
                           newSpell->getName() + Colors::RESET);
            state.resetTurnsSinceLastSpell();
        }
    } 

    if (state.getHand().isFull()) state.resetTurnsSinceLastSpell();;
}

// Пересоздать GameState
void Game::restartGameState() {
    int width = 15;
    int height = 10;
    int playerHealth = 100;
    int playerDamage = 20;
    
    state.clearGameState();
    new (&state) GameState(width, height, playerHealth, playerDamage);

    new (&combat) CombatSystem(state);

    initializeGame();

    clearFlags();
}

// Функция запроса названия файла для сохранения / загрузки
std::string Game::askFilename() {
    std::cout << Colors::YELLOW 
              << "Enter filename (WITHOUT path): "
              << Colors::RESET;

    std::string filename;
    std::getline(std::cin, filename);

    while (filename.empty()) {
        std::cout << Colors::RED << "Filename cannot be empty! Try again: "
                  << Colors::RESET;
        std::getline(std::cin, filename);
    }

    if (filename.find('/') != std::string::npos || filename.find('\\') != std::string::npos) {
        std::cout << Colors::RED
                  << "Do NOT include a path! Only the filename, like: save1.txt\n"
                  << Colors::RESET;
        return askFilename();
    }

    if (filename.find('.') == std::string::npos) {
        filename += ".txt";
    }

    return "../game/savings/" + filename;
}

// Основной цикл игры
void Game::run() {
    bool gameLoop = true; 
    GameVisualizer<ConsoleRenderer> visualizer(this);
    GameController<ConsoleInputProcessor> controller(this);

    while (gameLoop) {

        clearFlags();

        Player& player = state.getPlayer();
        const GameField& field = state.getField();

        visualizer.renderGame();
        
        // Основной цикл игры (пока пользователь не выйдет && игрок живой && есть враги)
        while (state.isGameRunning() && player.isAlive() && state.hasAliveEnemies()) {
            state.clearMessageLog(); // очищаем лог для новых сообщений
            
            // Считывание
            controller.processNextCommand();
            
            // Если прожата пауза
            if (isPauseRequested()) {
                showPauseMenu(); // показываем опции
                
                // Если запрошен выход - выходим
                if (isExitRequested()) {
                    state.setGameRunning(false);
                    gameLoop = false;
                    break;
                }

                // Если запрошена загрузка - загружаем игру (обновляем GameState) и продолжаем цикл
                if (isLoadRequested()) {
                    std::string fullpath = askFilename();
                    GameState backupState = state;

                    if (saveManager.load(state, fullpath)) {
                        std::cout << Colors::GREEN << "Game loaded successfully!" << Colors::RESET << std::endl;
                        state.setGameRunning(true);  
                        new (&combat) CombatSystem(state);
                    } else {
                        state = std::move(backupState); 
                        new (&combat) CombatSystem(state);
                        std::cout << Colors::RED << "Failed to load game. Previous game state restored." << Colors::RESET << std::endl;
                    }
                    std::cout << Colors::GREEN << "Press ENTER to continue..." << Colors::RESET;
                    std::cin.ignore(10000, '\n');
                }
                
                // Очищаем флаги, переходем к следущей итерации цикла
                clearFlags();
                visualizer.renderGame();
                continue;
            }

            // Ход + логика игрока
            combat.processPlayerCombat(); 
            
            // Добавляем заклинания
            state.incrementTurnsSinceLastSpell();
            tryGiveRandomSpell();
            
            // Ход + логика союзника
            combat.processAllyMovement();

            // Двигаем каждого врага + логика ходов + лоигрование результатов
            combat.processEnemyMovement();

            // Обновляем здания (Создаем врагов)
            for (auto& building : state.getAllBuildings()) {
                if (building->isAlive()) {
                    building->update(state, field, state.getLevel());
                }
            }

            // Обновляем башни (Атакуем игрока в допустимом интервале)
            for (auto& tower : state.getAllTowers()) {
                if (tower->isAlive()) {
                    tower->attackPlayerIfRange(player, state);
                }
            }

            visualizer.renderGame();
        }
 
        if (isExitRequested() || !state.isGameRunning() || !player.isAlive() || !state.hasAliveEnemies()) {
            gameLoop = false;
        }

    }
}

// Главное меню
void Game::showMainMenu() {
    bool inMainMenu = true;

    while (inMainMenu) {
        clearScreen();
        std::cout << Colors::CYAN << "=============================" << Colors::RESET << std::endl;
        std::cout << Colors::CYAN << "          MAIN MENU" << Colors::RESET << std::endl;
        std::cout << Colors::CYAN << "=============================" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "1 - Start New Game" << Colors::RESET << std::endl;
        std::cout << Colors::CYAN << "2 - Load Game" << Colors::RESET << std::endl;
        std::cout << Colors::RED << "3 - Exit" << Colors::RESET << std::endl;
        std::cout << Colors::CYAN << "=============================" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Choose option (1-3): " << Colors::RESET;
        
        int choice;
        if (std::cin >> choice) {
            std::cin.ignore(10000, '\n'); 
            
            switch (choice) {
                case 1: // Start New Game
                {
                    bool next = false;
                    restartGameState();
                    do {  
                        run(); // запускаем игровой цикл
                        next = showGameOver(); // Когда закончился цикл - показываем конец игры
                        if (next) {
                            nextLevel(); // Переходим на следующий уровень
                        }   
                    } while (next);
                    std::cout << Colors::GREEN << "Press ENTER to continue..." << Colors::RESET;
                    std::cin.ignore(10000, '\n');
                    break;
                } 
                case 2: // Load Game
                {   
                    requestLoad();
                    //state.clearGameState();
                    std::string fullpath = askFilename();
                    if (saveManager.load(state, fullpath)) {
                        std::cout << Colors::GREEN << "Game loaded successfully!" << Colors::RESET << std::endl;
                        state.setGameRunning(true);  
                        bool next = false;
                        do {
                            run();                
                            next = showGameOver(); 
                            if (next) {
                                nextLevel();       
                            }
                        } while (next);
                        std::cout << Colors::GREEN << "Press ENTER to continue..." << Colors::RESET;
                        std::cin.ignore(10000, '\n');
                        break;
                    } else {
                        std::cout << Colors::GREEN << "Press ENTER to continue..." << Colors::RESET;
                        std::cin.ignore(10000, '\n');
                        break;
                    }
                    break;
                }
                    
                case 3: // Exit
                    std::cout << Colors::GREEN << "Exiting game. Goodbye!" << Colors::RESET << std::endl;
                    inMainMenu = false;
                    break;
                    
                default:
                    std::cout << Colors::RED << "Invalid choice! Please select 1-3." << Colors::RESET << std::endl;
                    std::cout << "Press ENTER to continue...";
                    std::cin.ignore(10000, '\n');
                    break;
            }
        } else {
            std::cout << Colors::RED << "Invalid input! Please enter a number." << Colors::RESET << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Press ENTER to continue...";
            std::cin.ignore(10000, '\n');
        }
        
        // Если игра завершилась и мы вернулись в меню, сбрасываем флаги
        clearFlags();
    }
}

// Меню паузы
void Game::showPauseMenu() {
    //visualizer.clearScreen();
    clearScreen();
    std::cout << Colors::YELLOW << "===== PAUSE MENU =====" << Colors::RESET << std::endl;
    std::cout << "1 - Resume Game\n";
    std::cout << "2 - Save Game\n";
    std::cout << "3 - Load Game\n"; 
    std::cout << "4 - Exit to Main Menu\n";
    std::cout << Colors::GREEN << "Choose option (1-4): " << Colors::RESET;
    
    int choice;

    while (true) { 
        if (std::cin >> choice) {
            std::cin.ignore(10000, '\n'); 
            if (choice >= 1 && choice <= 4) break; 
            std::cout << Colors::RED << "Invalid choice! Please select 1-4: " << Colors::RESET;
        } else {
            std::cout << Colors::RED << "Invalid input! Please enter a number: " << Colors::RESET;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }

    switch (choice) {
        case 1: // Resume Game
            std::cout << Colors::GREEN << "Resuming game..." << Colors::RESET << std::endl;
            break;
        case 2: // Save Game
        {
            requestSave();
            std::string fullpath = askFilename();
            if (saveManager.save(state, fullpath)) {
                std::cout << Colors::GREEN << "Game saved successfully!" << Colors::RESET << std::endl;
            } 
            break;
        }
        case 3: // Load Game
            requestLoad();
            return;
        case 4: // Exit to Main Menu
            std::cout << Colors::YELLOW << "Returning to main menu..." << Colors::RESET << std::endl;
            requestExit();
            break;
        default:
            std::cout << Colors::RED << "Invalid choice! Please select 1-6." << Colors::RESET << std::endl;
            break;
    }
    
    std::cout << Colors::GREEN << "Press ENTER to continue..." << Colors::RESET;
    std::cin.ignore(10000, '\n');

}

// Переход на следующий уровень
void Game::nextLevel() {

    int newLevel = state.getLevel() + 1; 

    int newWidth = state.getField().getWidth() + 2;
    int newHeight = state.getField().getHeight() + 2;

    int playerHealth = state.getPlayer().getMaxHealth();
    int playerDamage = state.getPlayer().getDamage();

    auto hand = state.getHand().getAllSpells();
    std::random_shuffle(hand.begin(), hand.end());
    hand.resize(hand.size() / 2);

    state.clearGameState();
    new (&state) GameState(newWidth, newHeight, playerHealth, playerDamage);
    state.setLevel(newLevel);

    new (&combat) CombatSystem(state);

    initializeGame();

    for (auto& enemy : state.getAllCharacters()) {
        if (enemy != &state.getPlayer()) {
            enemy->setHealth(enemy->getHealth() + 10 * (state.getLevel() - 1));   // прибавляем здоровье
            enemy->setDamage(enemy->getDamage() + 5 * (state.getLevel() - 1));    // прибавляем урон
        }
    }

    for (auto& spell : hand) {
        state.addSpellToHand(spell);
    }

    clearFlags();
}

// Меню улучшения персонажа
void Game::levelUpMenu() {
    Player& player = state.getPlayer();
    while (true) {
        std::cout << Colors::CYAN << "===== LEVEL UP! =====" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "Choose a reward:\n"
                  << "1 - Increase Player Health (+20)\n"
                  << "2 - Increase Player Damage (+5)\n"
                  << "3 - Upgrade all spells in hand\n"
                  << Colors::CYAN << "Choose option: " << Colors::RESET;

        int choice;
        if (std::cin >> choice) {
            std::cin.ignore(10000, '\n');

            if (choice == 1) {
                player.setMaxHealth(player.getMaxHealth() + 20);
                player.setHealth(player.getMaxHealth());
                std::cout << Colors::GREEN << "Player health increased!" << Colors::RESET << std::endl;
                break;
            } 
            else if (choice == 2) {
                player.setDamage(player.getDamage() + 5);
                std::cout << Colors::GREEN << "Player damage increased!" << Colors::RESET << std::endl;
                break;
            } 
            else if (choice == 3) {
                auto hand = state.getHand().getAllSpells();
                for (auto& spell : hand) {
                    spell->upgrade();
                    std::cout << Colors::GREEN << spell->getName() << " upgraded!" << Colors::RESET << std::endl;
                }
                std::cout << Colors::GREEN << "All spells upgraded!" << Colors::RESET << std::endl;
                break;
            } 
        } else {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
    std::cout << Colors::GREEN << "Press ENTER to continue..." << Colors::RESET;
    std::cin.ignore(10000, '\n');
}

// Конец игры
bool Game::showGameOver() {
    Player& player = state.getPlayer();
    //visualizer.clearScreen();
    clearScreen();
    if (!player.isAlive()) {
        std::cout << Colors::DARK_RED << "===== GAME OVER =====" << Colors::RESET << std::endl;
        std::cout << Colors::DARK_RED << "You died, your score: " << player.getScore() << Colors::RESET << std::endl;
        return false;
    } else if (!state.hasAliveEnemies()) {
        std::cout << Colors::BRIGHT_GREEN << "===== YOU WIN! =====" << Colors::RESET << std::endl;
        std::cout << Colors::GREEN << "You killed all enemies! Your Score: " << player.getScore() << Colors::RESET << std::endl;
        levelUpMenu();
        while (true) {
            std::cout << Colors::CYAN << "Do you want to go to the next level?\n1 - Yes\n2 - No\nChoose: " << Colors::RESET;
            int choice;
            if (std::cin >> choice) {
                std::cin.ignore(10000, '\n');
                if (choice == 1) return true;
                if (choice == 2) return false;
            } else {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }
        }
    } else {
        std::cout << Colors::DARK_RED << "===== Leave the game =====" << Colors::RESET << std::endl;
        std::cout << Colors::ORANGE << "You left the game, your Score: " << player.getScore() << Colors::RESET << std::endl;
        return false;
    }
}

void Game::movePlayer(int dx, int dy) {
    Player& player = state.getPlayer();
    const GameField& field = state.getField();
    player.move(dx, dy, field);
}

void Game::switchCombatMode() {
    Player& player = state.getPlayer();
    player.switchCombatMode(state);
}

void Game::castSpell() {
    Player& player = state.getPlayer();
    const PlayerHand& hand = state.getHand();
    spellManager.spellProcessing(player, hand, state);
}

void Game::pauseGame() {
    requestPause();
}

void Game::requestExit() { exitRequested = true; }
void Game::requestPause() { pauseRequested = true; }
void Game::requestSave() { saveRequested = true; }
void Game::requestLoad() { loadRequested = true; }

bool Game::isExitRequested() const { return exitRequested; }
bool Game::isPauseRequested() const { return pauseRequested; }
bool Game::isSaveRequested() const { return saveRequested; }
bool Game::isLoadRequested() const { return loadRequested; }

void Game::clearFlags() {
    exitRequested = pauseRequested = saveRequested = loadRequested = false;
}

GameState& Game::getGameState() { return state; }

void Game::clearScreen() {
    #ifdef _WIN32
        system("cls");     // Windows
    #else
        system("clear");   // Linux/Mac
    #endif
}