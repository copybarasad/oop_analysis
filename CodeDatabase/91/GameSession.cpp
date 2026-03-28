#include "GameSession.h"
#include "Controllers/GameController.h"
#include "Input/ConsoleInputHandler.h"
#include "Visualizers/GameVisualizer.h"
#include "Renderers/ConsoleGameRenderer.h"

GameSession::GameSession() : currentState(GameState::MENU), currentGame(nullptr), currentLevel(1), logger_(nullptr), current_control_config_path_("") {
    std::cout << "=== Game Session Manager Initialized ===" << std::endl;

    current_control_config_path_ = default_control_config_path_;
    std::cout << "Configure controls:\n";
    std::cout << "Enter control config file path (or press Enter for default '" << default_control_config_path_ << "'): ";
    std::string user_input_path;
    std::getline(std::cin, user_input_path);
    if (!user_input_path.empty()) {
        current_control_config_path_ = user_input_path;
    }
    std::cout << "Controls will be loaded from: " << current_control_config_path_ << std::endl;
}

void GameSession::displayMainMenu() {
    std::cout << "\n=== MAIN MENU ===" << std::endl;
    std::cout << "N - Start New Game" << std::endl;
    std::cout << "L - Load Game" << std::endl;
    std::cout << "Q - Quit" << std::endl;
    std::cout << "Choose an option: ";
}

void GameSession::displayGameOverMenu() {
    std::cout << "\n=== GAME OVER ===" << std::endl;
    std::cout << "T - Try Again" << std::endl;
    std::cout << "M - Main Menu" << std::endl;
    std::cout << "Q - Quit" << std::endl;
    std::cout << "Choose an option: ";
}

char GameSession::getPlayerChoice() {
    char choice;
    std::cin >> choice;
    std::cin.ignore();
    return std::toupper(choice);
}

void GameSession::startNewGame(int fieldWidth, int fieldHeight) {
    Logger::OutputType log_output_type = Logger::OutputType::CONSOLE;
    std::string log_file_path = "game.log";

    std::cout << "Configure logging:\n";
    std::cout << "Enter 'C' for console logging, 'F' for file logging: ";
    std::string log_choice_input;
    std::getline(std::cin, log_choice_input);

    if (!log_choice_input.empty() && std::toupper(log_choice_input[0]) == 'F') {
        log_output_type = Logger::OutputType::FILE;
        std::cout << "Enter log file path (or press Enter for default 'game.log'): ";
        std::getline(std::cin, log_file_path);
        if (log_file_path.empty()) {
            log_file_path = "game.log";
        }
    }

    logger_ = std::make_unique<Logger>(log_output_type, log_file_path);
    std::cout << "Logger initialized.\n";

    try {
        currentGame = std::make_unique<Game>(fieldWidth, fieldHeight, logger_.get());
    } catch(const std::exception& e) {
        std::cerr << "Error starting new game: " << e.what() << std::endl;
        currentState = GameState::QUITTING;
    }

    if (currentGame) {
        runGameWithConfig(current_control_config_path_);
    }
}

void GameSession::runGameWithConfig(const std::string& config_path) {
    GameController<ConsoleInputHandler> controller(config_path);
    GameVisualizer<ConsoleGameRenderer> visualizer;
    GameResult result = controller.run(*currentGame, visualizer);

    if (result == GameResult::QUIT) {
        std::cout << "Press Enter to close the game window...";
        std::cin.ignore();
        std::cin.get();
        currentState = GameState::QUITTING;
        return;
    }

    if (result == GameResult::WIN) {
        startNextLevel();
    } else if (result == GameResult::LOSE) {
        currentState = GameState::GAME_OVER;
        currentGame.reset();
    }
}

void GameSession::startNextLevel() {
    if (!currentGame) {
        std::cerr << "Error: startNextLevel called without an active game!" << std::endl;
        currentState = GameState::MENU;
        return;
    }

    std::cout << "\n=== Level " << currentLevel << " Complete! ===" << std::endl;

    Player& current_player = currentGame->getPlayerRef();

    std::cout << "\nChoose an upgrade for Level " << (currentLevel + 1) << ":\n";
    std::cout << "1. Increase Max Health by 20 (" << current_player.getMaxHealth() << "/" << current_player.getMaxHealth() << " -> " << (current_player.getMaxHealth() + 20) << "/" << (current_player.getMaxHealth() + 20) << ")\n";
    std::cout << "2. Increase Base Damage by 15 (" << current_player.getDamage() << " -> " << (current_player.getDamage() + 15) << ")\n";
    std::cout << "3. Increase Max Mana by 20 (" << current_player.getMana() << "/" << current_player.getMaxMana() << " -> " << current_player.getMana() << "/" << (current_player.getMaxMana() + 20) << ")\n";
    std::cout << "4. Increase Spell Hand Size by 1 (" << current_player.getSpellHand().getMaxSize() << " -> " << (current_player.getSpellHand().getMaxSize() + 1) << ")\n";
    std::cout << "5. Upgrade a random spell in your hand (increases damage by ~50%)\n";
    std::cout << "Enter choice (1-5): ";

    int choice = 0;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1:
            current_player.upgradeMaxHealth(20);
            std::cout << "Max Health upgraded! Now: " << current_player.getMaxHealth() << std::endl;
            break;
        case 2:
            current_player.upgradeBaseDamage(15);
            std::cout << "Base Damage upgraded! Now: " << current_player.getDamage() << std::endl;
            break;
        case 3:
            current_player.upgradeMaxMana(20);
            std::cout << "Max Mana upgraded! Now: " << current_player.getMaxMana() << std::endl;
            break;
        case 4:
            current_player.getSpellHand().upgradeMaxSize(1);
            std::cout << "Spell Hand Size upgraded! Now: " << current_player.getSpellHand().getMaxSize() << std::endl;
            break;
        case 5: {
            SpellHand& hand = current_player.getSpellHand();
            if (!hand.isEmpty()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, hand.getSpellCount() - 1);
                int index = dis(gen);

                hand.upgradeSpellAt(index);
            } else {
                std::cout << "No spells to upgrade. Upgrade cancelled.\n";
            }
            break;
        }
        default:
            std::cout << "Invalid choice. No upgrade applied.\n";
            break;
    }

    currentLevel++;
    // std::cout << "\n=== Starting Level " << currentLevel << " ===" << std::endl;

    current_player.heal(current_player.getMaxHealth() - current_player.getHealth());
    std::cout << "Player health restored to full: " << current_player.getHealth() << "/" << current_player.getMaxHealth() << std::endl;

    int spells_to_remove = current_player.getSpellHand().getSpellCount() / 2;
    current_player.getSpellHand().removeRandomSpells(spells_to_remove);
    std::cout << "Removed " << spells_to_remove << " random spells from hand." << std::endl;

    int new_field_width = std::min(25, currentGame->getField().getWidth() + 1);
    int new_field_height = std::min(25, currentGame->getField().getHeight() + 1);

    // Обновляем игрока
    auto new_game = std::make_unique<Game>(new_field_width, new_field_height, logger_.get());
    new_game->setPlayerState(std::move(current_player));

    // Обновляем врагов и башни
    auto& new_enemies = new_game->getEnemiesRef();
    auto& new_towers = new_game->getTowersRef();

    for (auto& enemy : new_enemies) {
        int additional_health = currentLevel * 7.5;
        int additional_damage = currentLevel * 5;

        enemy.setMaxHealth(enemy.getMaxHealth() + additional_health);
        enemy.setHealth(enemy.getHealth() + additional_health);
        enemy.setDamage(enemy.getDamage() + additional_damage);
    }

    for (auto& tower_ptr : new_towers) {
        if (tower_ptr) {
            tower_ptr->applyLevelBonus(currentLevel);
        }
    }

    currentGame = std::move(new_game);

    if (logger_) {
        logger_->logNewLevelStarted(currentLevel, new_field_width, new_field_height);
    }

    std::cout << "Level " << currentLevel << " started! Field size: " << new_field_width << "x" << new_field_height << std::endl;

    currentState = GameState::PLAYING;
}

void GameSession::run() {
    while (currentState != GameState::QUITTING) {
        switch (currentState) {
            case GameState::MENU:
                displayMainMenu();
                switch (getPlayerChoice()) {
                    case 'N':
                        startNewGame();
                        break;
                    case 'L':
                        loadGame();
                        break;
                    case 'Q':
                        currentState = GameState::QUITTING;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        break;
                }
                break;

            case GameState::PLAYING:
                if (currentGame) {
                    runGameWithConfig(current_control_config_path_);
                } else {
                    std::cerr << "Error: PLAYING state but no active game!" << std::endl;
                    currentState = GameState::MENU;
                }
                break;

            case GameState::GAME_OVER:
                displayGameOverMenu();
                switch (getPlayerChoice()) {
                    case 'T':
                        startNewGame();
                        break;
                    case 'M':
                        currentState = GameState::MENU;
                        break;
                    case 'Q':
                        currentState = GameState::QUITTING;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                        break;
                }
                break;

            default:
                std::cerr << "Unexpected game state, returning to menu." << std::endl;
                currentState = GameState::MENU;
                break;
        }
    }
    std::cout << "\n=== Exiting Game ===" << std::endl;
}

void GameSession::loadGame() {
    std::string filepath;
    std::cout << "Enter load file path (<file_name>.json): ";
    std::getline(std::cin, filepath);

    if (filepath.empty()) {
        std::cout << "Invalid file path.\n";
        return;
    }

    std::unique_ptr<Game> loaded_game_ptr = nullptr;
    try {
        loaded_game_ptr = ::loadGame(filepath);
    } catch (const std::exception& e) {
        std::cout << "Failed to load game: " << e.what() << std::endl;
        currentState = GameState::MENU;
        return;
    }

    if (loaded_game_ptr) {
        currentGame = std::move(loaded_game_ptr);
        std::cout << "Game loaded successfully!\n";
        currentState = GameState::PLAYING;
    } else {
        std::cout << "Failed to load game. File might be corrupted or in wrong format.\n";
        currentState = GameState::MENU;
    }
}