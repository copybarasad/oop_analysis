#include "game_controller.h"
#include "game_state.h"
#include "logger.h"
#include "console_renderer.h"
#include "input_handler.h"
#include "visualizer.h"
#include "game_engine.h"
#include "level_manager.h"
#include "game_save.h"
#include "entity_manager.h"
#include "spell.h"
#include "event_manager.h"
#include <iostream>
#include <cstring>
#include <limits>
#include <vector>

GameController::GameController(int argc, char* argv[]) 
    : current_level(1), is_game_running(true) {
    
    int mode_int = parseLogMode(argc, argv);
    logger = std::make_shared<Logger>(static_cast<LogMode>(mode_int));
    
    EventManager::addObserver(logger.get());
}

GameController::~GameController() = default;

int GameController::parseLogMode(int argc, char* argv[]) {
    bool console = false;
    bool file = false;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-file") == 0) file = true;
        if (std::strcmp(argv[i], "-console") == 0) console = true;
        if (std::strcmp(argv[i], "-none") == 0) return 0;
    }
    if (file && console) return 3;
    if (file) return 2;
    if (console) return 1;
    return 1;
}

void GameController::rebuildArchitecture() {
    visualizer = std::make_unique<Visualizer<ConsoleRenderer>>(*game_state);
    engine = std::make_unique<GameEngine<InputHandler>>(*game_state);

    engine->addObserver(logger.get());
    engine->addObserver(visualizer.get());
    engine->getInputReader().loadConfig("keys.cfg");
}

void GameController::createNewLevel(int level, bool is_new_game) {
    int savedMaxLife = 3;
    int savedMeleeDamage = 2;
    int savedRangedDamage = 1;
    int savedSpellSlots = 3;
    int savedScore = 0;
    std::vector<std::unique_ptr<Spell>> savedSpells;

    if (!is_new_game && game_state) {
        Player& oldP = game_state->getPlayer();
        savedMaxLife = oldP.get_max_life();
        savedMeleeDamage = oldP.get_melee_damage();
        savedRangedDamage = oldP.get_ranged_damage();
        savedSpellSlots = oldP.get_spell_hand().get_max_size();
        savedScore = oldP.get_score();
        
        Hand& oldHand = oldP.get_spell_hand();
        for (int i = 0; i < oldHand.get_spell_count(); ++i) {
            savedSpells.push_back(std::unique_ptr<Spell>(oldHand.get_spell(i)->clone()));
        }
    }

    game_state = LevelManager::create_level(level);
    
    LevelManager::apply_level_modifiers(*game_state, level);
    
    if (!is_new_game) {
        Player& newP = game_state->getPlayer();
        newP.set_max_life(savedMaxLife);
        newP.set_current_life(savedMaxLife); 
        newP.set_melee_damage(savedMeleeDamage);
        newP.set_ranged_damage(savedRangedDamage);
        
        if (savedScore > 0) {
            Enemy dummy(-1, -1);
            dummy.set_alive(false);
            newP.getting_points(dummy, savedScore);
        }
        
        int currentSlots = newP.get_spell_hand().get_max_size();
        if (savedSpellSlots > currentSlots) {
            newP.increase_spell_slots(savedSpellSlots - currentSlots);
        }
        
        for (auto& spell : savedSpells) {
            if (!newP.get_spell_hand().is_full()) {
                newP.get_spell_hand().add_spell(std::move(spell));
            }
        }
        
        LevelManager::remove_half_spells(newP);
        
        newP.get_spell_hand().add_random_spell(game_state->getEntityManager());
    } else {
        if (game_state->getEnemies().empty()) {
             game_state->getEntityManager().spawnEnemies(3);
        }
    }

    rebuildArchitecture();
    
    std::cout << "Level " << level << " Started!" << std::endl;
}

void GameController::startNewGame() {
    current_level = 1;
    createNewLevel(current_level, true);
    processGameLoop();
}

bool GameController::loadGame() {
    try {
        auto result = GameSave::load_game("savegame.json");
        game_state = std::move(result.first);
        current_level = result.second;
        
        rebuildArchitecture();
        
        std::cout << "Game Loaded Successfully! Level " << current_level << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Load Failed: " << e.what() << std::endl;
        return false;
    }
}

void GameController::showMainMenu() {
    while (is_game_running) {
        std::cout << "\n=== MAIN MENU ===" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Option: ";
        
        int choice;
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        switch (choice) {
            case 1: startNewGame(); return;
            case 2: 
                if (loadGame()) processGameLoop(); 
                break;
            case 3: is_game_running = false; return;
            default: std::cout << "Invalid option!" << std::endl;
        }
    }
}

bool GameController::isLevelComplete() const {
    for (const auto& enemy : game_state->getEnemies()) {
        if (enemy.get_alive()) return false;
    }
    return true;
}

void GameController::showLevelComplete() {
    std::cout << "\n=== LEVEL " << current_level << " COMPLETE! ===" << std::endl;
    std::cout << "1. Continue" << std::endl;
    int c; std::cin >> c;
    
    showPlayerUpgrade();
    
    current_level++;
    createNewLevel(current_level, false);
    processGameLoop(); 
}

void GameController::showPlayerUpgrade() {
    std::cout << "\n=== UPGRADE ===" << std::endl;
    std::cout << "1. +1 Max Health" << std::endl;
    std::cout << "2. +1 Melee Damage" << std::endl;
    std::cout << "3. +1 Ranged Damage" << std::endl;
    std::cout << "4. +1 Spell Slot" << std::endl;
    std::cout << "Choose: ";
    
    int choice;
    std::cin >> choice;
    LevelManager::upgrade_player(game_state->getPlayer(), choice);
}

void GameController::showGameOver() {
    std::cout << "\n=== GAME OVER ===" << std::endl;
    std::cout << "Final Score: " << game_state->getPlayer().get_score() << std::endl;
    std::cout << "1. Restart" << std::endl;
    std::cout << "2. Main Menu" << std::endl;
    
    int choice;
    std::cin >> choice;
    
    if (choice == 1) startNewGame();
}

void GameController::processGameLoop() {
    visualizer->forceDraw();

    while (true) {
        bool engine_running = engine->update();
        
        if (engine->isLoadRequested()) {
            if (loadGame()) {
                visualizer->forceDraw();
                continue; 
            } else {
                continue;
            }
        }
        
        if (!engine_running) {
            if (game_state->getPlayer().get_curr_life() <= 0) {
                showGameOver();
                return; 
            } else {
                return; 
            }
        }
        
        if (isLevelComplete()) {
            showLevelComplete();
            return; 
        }
    }
}

int GameController::run() {
    while (is_game_running) {
        showMainMenu();
    }
    return 0;
}