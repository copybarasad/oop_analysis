#include "Game.hpp"
#include <iostream>
#include <string>
#include <sstream>

// Инициализация игры с начальным состоянием меню
// Теперь использует шаблонные контроллер и визуализатор
Game::Game()
    : field(), currentState(State::MAIN_MENU), 
      currentLevel(1), gameRunning(true),
      controller(), visualizer() {}

// Главный игровой цикл
void Game::run() {
    while (gameRunning) {
        switch (currentState) {
            case State::MAIN_MENU:
                handleMainMenu();
                break;
            case State::PLAYING:
                handlePlaying();
                break;
            case State::GAME_OVER:
                handleGameOver();
                break;
            case State::VICTORY:
                handleVictory();
                break;
            case State::LOAD_MENU:
                handleLoadMenu();
                break;
            case State::PAUSE_MENU:
                handlePauseMenu();
                break;
        }
    }
}

// Обработка главного меню
void Game::handleMainMenu() {
    visualizer.renderMainMenu();
    
    Command command = controller.getCommand();
    
    switch (command.type) {
        case Command::Type::MENU_SELECT:
            if (command.menuIndex == 0) {
                startNewGame();
            } else if (command.menuIndex == 1) {
                currentState = State::LOAD_MENU;
            } else if (command.menuIndex == 2) {
                gameRunning = false;
            }
            break;
        case Command::Type::QUIT:
            gameRunning = false;
            break;
        default:
            break;
    }
}

// Обработка игрового процесса
void Game::handlePlaying() {
    // Отрисовка текущего состояния через визуализатор
    visualizer.renderGameState(field, field.getPlayer());

    // Проверка условий поражения и победы
    if (!field.getPlayer().isAlive()) {
        currentState = State::GAME_OVER;
        return;
    }

    if (field.getEnemiesWithPositions().empty()) {
        currentState = State::VICTORY;
        return;
    }

    Command command = controller.getCommand();
    
    if (std::cin.eof() || std::cin.fail()) {
        std::cout << std::endl << "Thanks for playing!" << std::endl;
        gameRunning = false;
        return;
    }

    bool actionTaken = false;

    // Обработка игровых команд
    switch (command.type) {
        case Command::Type::MOVE:
            actionTaken = field.movePlayer(command.direction);
            break;
        case Command::Type::ATTACK:
            field.playerAttack(command.direction);
            actionTaken = true;
            break;
        case Command::Type::SWITCH_COMBAT_MODE: {
            std::string message = field.getPlayer().switchCombatMode();
            std::cout << message << std::endl;
            actionTaken = true;
            break;
        }
        case Command::Type::CAST_SPELL: {
            int idx = command.spellIndex;

            try {
                std::cout << "Enter target coordinates (x y): ";
                std::string coordsLine;
                if (!std::getline(std::cin, coordsLine)) {
                    throw std::runtime_error("Failed to read coordinates from input");
                }
                
                std::istringstream iss(coordsLine);
                int tx, ty;
                if (!(iss >> tx >> ty)) {
                    throw std::invalid_argument("Invalid coordinates format. Expected two integers.");
                }
                
                // Проверка валидности координат
                if (tx < 0 || ty < 0) {
                    throw std::out_of_range("Coordinates cannot be negative");
                }
                
                Pos target(tx, ty);
                Hand& hand = field.getPlayer().getHand();
                
                // Проверка валидности индекса заклинания
                if (idx < 0 || static_cast<size_t>(idx) >= hand.size()) {
                    throw std::out_of_range("Invalid spell index");
                }
                
                bool used = hand.useSpell(static_cast<size_t>(idx), field, field.getPlayerPosition(), target);
                actionTaken = used;
                
                if (!used) {
                    std::cout << "Spell was not used." << std::endl;
                }
                
            } catch (const std::exception& e) {
                std::cout << "Error casting spell: " << e.what() << std::endl;
                actionTaken = false;
                // Программа продолжает работу
            }
            break;
        }
        case Command::Type::SAVE_GAME:
            saveGame();
            break;
        case Command::Type::MAIN_MENU:
            currentState = State::PAUSE_MENU;
            break;
        case Command::Type::QUIT:
            gameRunning = false;
            break;
        case Command::Type::NONE:
        default:
            actionTaken = false;
            break;
    }

    // Ход врагов после действия игрока
    if (actionTaken) {
        field.moveEnemies();
    }
}

// Обработка экрана поражения
void Game::handleGameOver() {
    visualizer.renderGameOver(false, field.getPlayer().getScore());
    
    std::cout << "\n1. Start Menu" << std::endl;
    std::cout << "2. Quit" << std::endl;
    std::cout << "Select option (1-2): ";
    
    Command command = controller.getCommand();
    if (command.type == Command::Type::MENU_SELECT) {
        if (command.menuIndex == 0) {
            currentState = State::MAIN_MENU;
        } else if (command.menuIndex == 1) {
            gameRunning = false;
        }
    } else if (command.type == Command::Type::QUIT) {
        gameRunning = false;
    }
}

// Обработка экрана победы
void Game::handleVictory() {
    visualizer.renderGameOver(true, field.getPlayer().getScore());
    
    if (currentLevel < Config::Settings::MAX_LEVELS) {
        std::cout << "\n1. Next Level" << std::endl;
        std::cout << "2. Main Menu" << std::endl;
        std::cout << "3. Quit" << std::endl;
        std::cout << "Select option (1-3): ";
        
        Command command = controller.getCommand();
        if (command.type == Command::Type::MENU_SELECT) {
            if (command.menuIndex == 0) {
                nextLevel();
            } else if (command.menuIndex == 1) {
                currentState = State::MAIN_MENU;
            } else if (command.menuIndex == 2) {
                gameRunning = false;
            }
        } else if (command.type == Command::Type::QUIT) {
            gameRunning = false;
        }
    } else {
        std::cout << "\nYou have completed all levels! You are the champion!" << std::endl;
        std::cout << "1. Main Menu" << std::endl;
        std::cout << "2. Quit" << std::endl;
        std::cout << "Select option (1-2): ";
        
        Command command = controller.getCommand();
        if (command.type == Command::Type::MENU_SELECT) {
            if (command.menuIndex == 0) {
                currentState = State::MAIN_MENU;
            } else if (command.menuIndex == 1) {
                gameRunning = false;
            }
        } else if (command.type == Command::Type::QUIT) {
            gameRunning = false;
        }
    }
}

// Обработка меню паузы
void Game::handlePauseMenu() {
    visualizer.renderPauseMenu();
    Command command = controller.getCommand();
    
    switch (command.type) {
        case Command::Type::MENU_SELECT:
            if (command.menuIndex == 0) {
                currentState = State::PLAYING;
            } else if (command.menuIndex == 1) {
                saveGame();
            } else if (command.menuIndex == 2) {
                currentState = State::MAIN_MENU;
            } else if (command.menuIndex == 3) {
                gameRunning = false;
            }
            break;
        case Command::Type::QUIT:
            gameRunning = false;
            break;
        default:
            break;
    }
}

// Обработка меню загрузки
void Game::handleLoadMenu() {
    try {
        auto saves = SaveManager::getSaveFiles();  // Сохраняем результат в переменную
        visualizer.renderLoadMenu(saves);
        
        Command command = controller.getCommand();
        
        if (command.type == Command::Type::MENU_SELECT) {
            int selectedIndex = command.menuIndex;
            if (selectedIndex >= 0 && selectedIndex < static_cast<int>(saves.size())) {
                std::string saveName = saves[selectedIndex].filename;
                if (SaveManager::loadGame(saveName, field, currentLevel)) {
                    currentState = State::PLAYING;
                    std::cout << "Game loaded. Level " << currentLevel << std::endl;
                } else {
                    throw std::runtime_error("Failed to load game data");
                }
            } else {
                throw std::out_of_range("Invalid save file selection");
            }
        } else if (command.type == Command::Type::MAIN_MENU) {
            currentState = State::MAIN_MENU;
        }
    } catch (const std::exception& e) {
        std::cout << "Error in load menu: " << e.what() << std::endl;
        currentState = State::MAIN_MENU;
    }
}

// Начало новой игры
void Game::startNewGame() {
    try {
        currentLevel = 1;
        field = Field(Config::Levels::LEVEL_1);
        currentState = State::PLAYING;
        std::cout << "Starting new game. Level " << currentLevel << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error starting new game: " << e.what() << std::endl;
        currentState = State::MAIN_MENU;
    }
}

// Загрузка игры
void Game::loadGame() {
    auto saves = SaveManager::getSaveFiles();
    if (saves.empty()) {
        std::cout << "No save files found." << std::endl;
        currentState = State::MAIN_MENU;
        return;
    }
    
    std::string saveName = saves[0].filename;
    if (SaveManager::loadGame(saveName, field, currentLevel)) {
        currentState = State::PLAYING;
        std::cout << "Game loaded. Level " << currentLevel << std::endl;
    } else {
        std::cout << "Failed to load game." << std::endl;
        currentState = State::MAIN_MENU;
    }
}

// Сохранение игры
void Game::saveGame() {
    try {
        std::string saveName = SaveManager::generateSaveName();
        if (SaveManager::saveGame(saveName, field, currentLevel)) {
            std::cout << "Game saved as: " << saveName << std::endl;
        } else {
            throw std::runtime_error("Save manager returned failure");
        }
    } catch (const std::exception& e) {
        std::cout << "Failed to save game: " << e.what() << std::endl;
    }
}

// Переход на следующий уровень
void Game::nextLevel() {
    // Сохраняем состояние игрока перед созданием нового уровня
    Player& currentPlayer = field.getPlayer();
    int savedScore = currentPlayer.getScore();
    bool savedCombatMode = currentPlayer.getIsRangedMode();
    
    Hand savedHand = currentPlayer.getHand();

    currentLevel++;
    if (currentLevel > Config::Settings::MAX_LEVELS) {
        currentLevel = Config::Settings::MAX_LEVELS;
        currentState = State::VICTORY;
        return;
    }
    
    // Создаем новый уровень
    LevelConfig config;
    switch (currentLevel) {
        case 1: config = Config::Levels::LEVEL_1; break;
        case 2: config = Config::Levels::LEVEL_2; break;
        case 3: config = Config::Levels::LEVEL_3; break;
        default: config = Config::Levels::LEVEL_1; break;
    }
    
    field = Field(config);
    
    // Восстанавливаем состояние игрока
    Player& newPlayer = field.getPlayer();
    newPlayer.setScore(savedScore);
    newPlayer.setRangedMode(savedCombatMode);
    
    // Восстанавливаем здоровье
    newPlayer.setHealth(Config::Settings::PLAYER_START_HEALTH);
    
    // Переносим заклинания и удаляем половину
    Hand& newHand = newPlayer.getHand();

    // Очищаем руку нового игрока (удаляем стартовые заклинания)
    while (newHand.size() > 0) {
        newHand.removeSpell(0);
    }
    
    // Копируем заклинания из сохраненной руки
    std::vector<std::unique_ptr<ISpell>> spellsToCopy;
    for (size_t i = 0; i < savedHand.size(); ++i) {
        const ISpell* spell = savedHand.getSpell(i);
        if (spell) {
            spellsToCopy.push_back(std::unique_ptr<ISpell>(spell->clone()));
        }
    }

    // Добавляем заклинания в новую руку
    for (auto& spell : spellsToCopy) {
        if (newHand.size() < newHand.capacity()) {
            newHand.addSpell(std::move(spell));
        }
    }
    
    // Удаляем половину карточек (случайным образом)
    size_t initialSpellCount = newHand.size();
    size_t cardsToRemove = initialSpellCount / 2;
    
    for (size_t i = 0; i < cardsToRemove && newHand.size() > 0; ++i) {
        size_t randomIndex = rand() % newHand.size();
        newHand.removeSpell(randomIndex);
    }
    
    currentState = State::PLAYING;
    std::cout << "Welcome to level " << currentLevel << "! Your health has been restored. " 
              << cardsToRemove << " spells have been removed from " << initialSpellCount << " total." << std::endl;
}
