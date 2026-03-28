#include "App.h"
#include "game/Game.h"
#include "game/GameManager.h"
#include "game/input/ConsoleInputHandler.h"
#include "game/rendering/ConsoleRenderer.h"
#include "game/observers/GameVisualizer.h"
#include "game/observers/LoggerSystem.h"
#include "game/observers/ConsoleLogger.h"
#include "game/observers/FileLogger.h"
#include "game/GameSaver.h"
#include "game/Exceptions.h"
#include "game/SpellHand.h"
#include <iostream>
#include <limits>
#include <string_view>

namespace Game {

App::App(int argc, char* argv[]) : level(1), scoreToWin(1000), loggerType("console") {
    // Загрузка конфигурации происходит здесь, один раз при запуске.
    // Сообщения об успехе или ошибке будут видны в консоли до старта игры.
    keyConfig.loadFromFile("controls.cfg");

    if (argc > 1) {
        std::string_view arg = argv[1];
        if (arg == "--log-file") {
            loggerType = "file";
            std::cout << "File logging enabled. Output will be written to log.txt" << std::endl;
        }
    }
}

App::~App() = default;

void App::run() {
    mainLoop();
}

void App::mainLoop() {
    bool running = true;
    while(running) {
        char choice = showMainMenu();
        switch(choice) {
            case '1':
                startNewGame();
                runGameSession();
                break;
            case '2':
                try {
                    loadGame();
                    std::cout << "Game loaded successfully!" << std::endl;
                    runGameSession();
                } catch (const SaveLoadException& e) {
                    std::cerr << "Load failed: " << e.what() << std::endl;
                }
                break;
            case '3':
                running = false;
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
                break;
        }
    }
    std::cout << "Goodbye!" << std::endl;
}

void App::runGameSession() {
    bool sessionActive = true;
    while(sessionActive) {
        if (!currentLevel) {
            currentLevel = std::make_unique<Game>(level, player, scoreToWin);
        }
        
        ConsoleRenderer renderer;
        
        std::unique_ptr<ILogger> logger;
        bool pauseForLogs = false;
        if (loggerType == "file") {
            try {
                logger = std::make_unique<FileLogger>("log.txt");
            } catch (const FileOpenException& e) {
                std::cerr << "Critical Error: Could not open log file! " << e.what() << std::endl;
                return;
            }
        } else {
            logger = std::make_unique<ConsoleLogger>();
            pauseForLogs = true;
        }
        LoggerSystem loggerSystem(std::move(logger));
        currentLevel->addObserver(&loggerSystem);

        // Передаем уже загруженную конфигурацию в GameManager
        GameManager<ConsoleInputHandler, ConsoleRenderer> gameManager(*currentLevel, renderer, pauseForLogs, keyConfig);
        
        LevelResult result = gameManager.run();
        
        switch (result) {
            case LevelResult::Win:
                level++;
                scoreToWin *= 2;
                handlePlayerUpgrade();
                player.restoreHealth();
                player.getSpellHand().removeRandomSpells(player.getSpellHand().getSpellCount() / 2);
                currentLevel.reset();
                break;
            case LevelResult::Loss:
                if (askToTryAgain()) {
                    startNewGame();
                } else {
                    sessionActive = false;
                }
                currentLevel.reset();
                break;
            case LevelResult::SaveAndQuit:
                try {
                    GameSaver::save(*currentLevel, player, saveFilename);
                    currentLevel->notify({EventType::GameSaved, std::monostate{}});
                } catch (const SaveLoadException& e) {
                    std::cerr << "Save failed: " << e.what() << std::endl;
                }
                currentLevel.reset();
                sessionActive = false;
                break;
            case LevelResult::Quit:
                currentLevel.reset();
                sessionActive = false;
                break;
            default:
                currentLevel.reset();
                sessionActive = false;
                break;
        }
    }
}

void App::startNewGame() {
    level = 1;
    scoreToWin = 1000;
    player = Player();
    player.getSpellHand().addSpell(SpellHand::createRandomSpell());
    currentLevel.reset();
}

void App::loadGame() {
    currentLevel = GameSaver::load(player, saveFilename);
    level = currentLevel->getLevelNumber();
    scoreToWin = currentLevel->getScoreToWin();
}

void App::handlePlayerUpgrade() {
    std::cout << "\n--- CHOOSE AN UPGRADE ---" << std::endl;
    std::cout << "1. Increase Max Health (+20)" << std::endl;
    std::cout << "2. Increase Melee Damage (+5)" << std::endl;
    std::cout << "3. Increase Ranged Damage (+5)" << std::endl;
    std::cout << "Your choice: ";
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    player.upgrade(choice - '0');
}

char App::showMainMenu() {
    std::cout << "\n--- MAIN MENU ---" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Your choice: ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

bool App::askToTryAgain() {
    char choice;
    while (true) {
        std::cout << "Start over from scratch? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'y' || choice == 'Y') return true;
        if (choice == 'n' || choice == 'N') return false;
    }
}

}