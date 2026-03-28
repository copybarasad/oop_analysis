#include "game_application.h"
#include "game_saver.h"
#include "file_logger.h"
#include "command_input.h"
#include "game_renderer.h"
#include "game_controller.h"
#include "game_visualizer.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>

GameApplication::GameApplication() 
    : event_system_(nullptr), 
      file_logger_(nullptr), 
      console_logger_(nullptr),
      current_game_(nullptr),
      is_running_(false) {  
    std::srand(std::time(0));
}

GameApplication::~GameApplication() {
    cleanup();
}

void GameApplication::stop() {
    is_running_ = false;  
}

void GameApplication::cleanup() {
    is_running_ = false; 
    
    if (file_logger_) {
        file_logger_->shutdown();
        delete file_logger_;
        file_logger_ = nullptr;
    }
    
    if (console_logger_) {
        delete console_logger_;
        console_logger_ = nullptr;
    }
    
    if (event_system_) {
        delete event_system_;
        event_system_ = nullptr;
    }
    
    if (current_game_) {
        delete current_game_;
        current_game_ = nullptr;
    }
}

int GameApplication::getLoggingChoice() {
    std::cout << "\n==============================\n";
    std::cout << "  НАСТРОЙКА ЛОГИРОВАНИЯ\n";
    std::cout << "==============================\n";
    std::cout << "Выберите способ логирования:\n";
    std::cout << "1. Только консоль (рекомендуется)\n";
    std::cout << "2. Только файл (game_log.txt)\n";
    std::cout << "3. Консоль и файл\n";
    std::cout << "Ваш выбор (1-3, Enter для 1): ";
    
    std::string logChoiceStr;
    std::getline(std::cin, logChoiceStr);
    
    if (logChoiceStr.empty()) {
        return 1;
    }
    
    try {
        int choice = std::stoi(logChoiceStr);
        if (choice >= 1 && choice <= 3) {
            return choice;
        }
        return 1;
    } catch (...) {
        return 1;
    }
}

void GameApplication::setupLogging(int log_choice) {
    event_system_ = new EventSystem();
    
    if (log_choice == 2) {
        file_logger_ = new FileLogger("game_log.txt");
        event_system_->subscribe(file_logger_);
        std::cout << "Логирование: только файл (game_log.txt)\n";
    }
    else if (log_choice == 3) {
        file_logger_ = new FileLogger("game_log.txt");
        console_logger_ = new ConsoleLogger();
        event_system_->subscribe(file_logger_);
        event_system_->subscribe(console_logger_);
        std::cout << "Логирование: консоль и файл\n";
    }
    else {
        console_logger_ = new ConsoleLogger();
        event_system_->subscribe(console_logger_);
        std::cout << "Логирование: только консоль\n";
    }
    
    std::cout << "==============================\n\n";
}

bool GameApplication::promptForRestart() {
    std::cout << "\nХотите начать заново? (Y/N): ";
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "Y" || choice == "y") {
        return true;
    }
    
    std::cout << "\nДо свидания!\n";
    return false;
}

std::string GameApplication::getPlayerName() const {
    std::string hunterName;
    std::cout << "Введите имя охотника (или нажмите Enter для 'Ван Хельсинг'): ";
    std::getline(std::cin, hunterName);
    
    if (hunterName.empty()) {
        hunterName = "Ван Хельсинг";
    }
    return hunterName;
}

void GameApplication::showWelcomeMessage() {
    std::cout << "\n🦇══════════════════════════════════════🦇\n";
    std::cout << "         ОХОТНИК НА ВАМПИРОВ\n"; 
    std::cout << "🦇══════════════════════════════════════🦇\n\n";
}

void GameApplication::runGameWithControllers(Game& game) {
    try {
        GameController<CommandInput> controller(game);
        GameVisualizer<GameRenderer> visualizer(game);
        
        visualizer.displayWelcome();
        visualizer.displayLevelInfo();
        
        game.startGame();
        
        while (game.isGameRunning() && is_running_) {  
            visualizer.displayQuickHelp();
            
            if (controller.processPlayerTurn()) {
                game.processEnemyTurn();
            }
        }
        
        if (game.isGameRunning() == false) {
            const auto& dracula = game.getDracula();
            const auto& hunter = game.getHunter();
            
            if (!dracula.isAlive()) {
                visualizer.displayMessage("ПОБЕДА! Дракула уничтожен навсегда!");
            } else if (!hunter.isAlive()) {
                visualizer.displayMessage("ПОРАЖЕНИЕ! Охотник погиб...");
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка в runGameWithControllers: " << e.what() << std::endl;
        throw;
    }
}

bool GameApplication::handleSaveGame(const std::string& hunter_name) {
    GameSaver saver("savegame.vhs");
    
    if (!saver.saveExists()) {
        return false;
    }
    
    std::cout << "\n💾 Обнаружено сохранение игры!\n";
    std::cout << "Выберите действие:\n";
    std::cout << "1. Загрузить сохраненную игру\n";
    std::cout << "2. Начать новую игру\n";
    std::cout << "Ваш выбор (1 или 2): ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice != "1") {
        return false;
    }
    
    try {
        current_game_ = new Game(10, 10, hunter_name, event_system_);
        saver.loadGame(*current_game_);
        std::cout << "\nИгра загружена успешно!\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки: " << e.what() << "\n";
        std::cout << "Будет запущена новая игра.\n\n";
        if (current_game_) {
            delete current_game_;
            current_game_ = nullptr;
        }
        return false;
    }
}

void GameApplication::startNewGame(const std::string& hunter_name) {
    std::cout << "\n🎮 Начинаем новую игру!\n";
    std::cout << "Охотник: " << hunter_name << std::endl;
    std::cout << "Противник: Дракула 🧛\n";
    std::cout << "Уровней: 6 с прогрессирующей сложностью\n";
    std::cout << "\nНажмите Enter для начала...";
    std::string temp;
    std::getline(std::cin, temp);
    
    current_game_ = new Game(10, 10, hunter_name, event_system_);
}

void GameApplication::playGameSession() {
    std::string hunter_name = getPlayerName();
    
    bool load_successful = handleSaveGame(hunter_name);
    
    if (!load_successful) {
        startNewGame(hunter_name);
    }
    
    if (current_game_) {
        runGameWithControllers(*current_game_);
        
        if (file_logger_) {
            file_logger_->shutdown();
        }
        
        delete current_game_;
        current_game_ = nullptr;
    }
}

void GameApplication::run() {
    try {
        is_running_ = true;  
        
        showWelcomeMessage();
        int log_choice = getLoggingChoice();
        setupLogging(log_choice);
        
        while (is_running_) {  
            playGameSession();
            
            if (is_running_) {  
                bool restart = promptForRestart();
                if (!restart) {
                    stop();  
                } else {
                    std::cout << "\n" << std::string(50, '=') << "\n";
                    std::cout << "  ЗАПУСК НОВОЙ ИГРЫ\n";
                    std::cout << std::string(50, '=') << "\n\n";
                }
            }
        }
        
    } catch (const std::exception& e) {
        std::cout << "Критическая ошибка: " << e.what() << std::endl;
        std::cout << "Перезапустите программу.\n";
        stop();  
    }
    
    cleanup();  
}