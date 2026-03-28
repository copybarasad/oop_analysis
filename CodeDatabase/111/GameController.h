#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "CommandInput.h"
#include "GameVisualizer.h"
#include "GameSession.h"
#include <iostream>
#include <memory>
#include <limits>

template<typename InputStrategy = DefaultInputStrategy, typename VisualStrategy = ConsoleVisualization>
class GameController {
private:
    CommandInput<InputStrategy> commandInput;
    GameVisualizer<VisualStrategy> visualizer;
    GameSession* gameSession;
    bool running;
    
    void updateVisualization() {
        if (!gameSession) return;
        
        const GameBoard& board = gameSession->getBoard();
        const Hero& hero = gameSession->getHero();
        const auto& enemies = gameSession->getEnemies();
        const auto& towers = gameSession->getTowers();
        
        visualizer.update(board, hero, enemies, towers);
    }
    
    bool processCommand(const PlayerCommand& cmd) {
        if (!gameSession) return false;
        
        std::cout << "GAMECONTROLLER DEBUG: Processing command type: " << static_cast<int>(cmd.type) << "\n";
        
        switch (cmd.type) {
            case CommandType::MOVE:
                std::cout << "GAMECONTROLLER DEBUG: Processing MOVE command\n";
                gameSession->getHero().performMove(cmd.direction, gameSession->getBoard());
                gameSession->checkCollisions();
                gameSession->endPlayerTurn();
                return true;
                
            case CommandType::SWITCH_COMBAT:
                std::cout << "GAMECONTROLLER DEBUG: Processing SWITCH_COMBAT command\n";
                gameSession->getHero().toggleCombatStyle();
                visualizer.showMessage("Combat style switched!");
                return true;
                
            case CommandType::RANGED_ATTACK:
                std::cout << "GAMECONTROLLER DEBUG: Processing RANGED_ATTACK command\n";
                if (gameSession->getHero().performRangedAttack(gameSession->getBoard())) {
                    visualizer.showMessage("Ranged attack executed!");
                    gameSession->endPlayerTurn();
                } else {
                    visualizer.showMessage("No valid target for ranged attack!");
                }
                return true;
                
            case CommandType::CAST_SPELL: {
                std::cout << "GAMECONTROLLER DEBUG: Processing CAST_SPELL command\n";
                std::cout << "GAMECONTROLLER DEBUG: Spell index: " << cmd.spellIndex 
                          << ", target: (" << cmd.target.row << "," << cmd.target.col << ")\n";
                
                if (cmd.spellIndex < 0) {
                    std::cout << "GAMECONTROLLER DEBUG: Invalid spell index\n";
                    visualizer.showMessage("Invalid spell index!");
                    return true;
                }
                
                Hero& hero = gameSession->getHero();
                std::cout << "GAMECONTROLLER DEBUG: Hero has " << hero.getSpellHand().getSpellCount() << " spells\n";
                
                if (cmd.spellIndex >= static_cast<int>(hero.getSpellHand().getSpellCount())) {
                    std::cout << "GAMECONTROLLER DEBUG: Spell index out of range\n";
                    visualizer.showMessage("No spell in that slot!");
                    return true;
                }
                
                std::cout << "GAMECONTROLLER DEBUG: Calling hero.castSpell()\n";
                try {
                    hero.castSpell(cmd.spellIndex, cmd.target, gameSession->getBoard());
                } catch (const std::exception& e) {
                    std::cout << "GAMECONTROLLER DEBUG: Exception during spell cast: " << e.what() << "\n";
                    visualizer.showMessage("Error casting spell!");
                    return true;
                } catch (...) {
                    std::cout << "GAMECONTROLLER DEBUG: Unknown exception during spell cast\n";
                    visualizer.showMessage("Unknown error casting spell!");
                    return true;
                }
                
                std::cout << "GAMECONTROLLER DEBUG: Calling endPlayerTurn()\n";
                gameSession->endPlayerTurn();
                
                std::cout << "GAMECONTROLLER DEBUG: CAST_SPELL processing completed\n";
                return true;
            }
                
            case CommandType::OPEN_SHOP:
                std::cout << "GAMECONTROLLER DEBUG: Processing OPEN_SHOP command\n";
                visualizer.showMessage("Opening shop...");
                gameSession->openShop();
                return true;
                
            case CommandType::SHOW_INFO:
                std::cout << "GAMECONTROLLER DEBUG: Processing SHOW_INFO command\n";
                showSpellInfo();
                commandInput.showControls();
                return true;
                
            case CommandType::SAVE_GAME:
                std::cout << "GAMECONTROLLER DEBUG: Processing SAVE_GAME command\n";
                if (!cmd.filename.empty()) {
                    gameSession->saveGame(cmd.filename);
                    visualizer.showMessage("Game saved!");
                } else {
                    visualizer.showMessage("Please specify filename!");
                }
                return true;
                
            case CommandType::LOAD_GAME:
                std::cout << "GAMECONTROLLER DEBUG: Processing LOAD_GAME command\n";
                visualizer.showMessage("Please use main menu to load game");
                return true;
                
            case CommandType::MAIN_MENU:
                std::cout << "GAMECONTROLLER DEBUG: Processing MAIN_MENU command\n";
                visualizer.showMessage("Returning to main menu...");
                return false;
                
            case CommandType::EXIT_GAME:
                std::cout << "GAMECONTROLLER DEBUG: Processing EXIT_GAME command\n";
                visualizer.showMessage("Exiting game...");
                return false;
                
            case CommandType::INVALID:
                std::cout << "GAMECONTROLLER DEBUG: Processing INVALID command\n";
                visualizer.showMessage("Invalid command!");
                return true;
                
            default:
                std::cout << "GAMECONTROLLER DEBUG: Processing unknown command type\n";
                return true;
        }
    }
    
    void showSpellInfo() {
        if (!gameSession) return;
        
        const Hero& hero = gameSession->getHero();
        const SpellHand& spellHand = hero.getSpellHand();
        
        std::cout << "\n--- SPELL INFORMATION ---\n";
        std::cout << "Your mana: " << spellHand.getMana() << " (used to buy spells in shop)\n";
        std::cout << "Spells in hand (" << spellHand.getSpellCount() 
                  << "/" << spellHand.getMaxSize() << "):\n";
        
        if (spellHand.getSpellCount() == 0) {
            std::cout << "  No spells - visit the shop to buy some!\n";
        } else {
            for (size_t i = 0; i < spellHand.getSpellCount(); ++i) {
                const Spell* spell = spellHand.getSpell(i);
                if (spell) {
                    std::cout << "  m" << (i + 1) << ". " << spell->getName() 
                              << " - " << spell->getDescription()
                              << " [Range: " << spell->getRange() 
                              << ", Shop Cost: " << spell->getManaCost() << "]\n";
                } else {
                    std::cout << "  m" << (i + 1) << ". [ERROR: Spell is null!]\n";
                }
            }
        }
        std::cout << "-------------------------\n";
    }
    
public:
    GameController(GameSession* session = nullptr) 
        : gameSession(session), running(false) {
        std::cout << "GAMECONTROLLER DEBUG: GameController created\n";
    }
    
    ~GameController() {
        std::cout << "GAMECONTROLLER DEBUG: GameController destroyed\n";
    }
    
    GameController(const GameController&) = delete;
    GameController& operator=(const GameController&) = delete;
    
    void setGameSession(GameSession* session) {
        std::cout << "GAMECONTROLLER DEBUG: Setting game session\n";
        gameSession = session;
    }
    
    void run() {
        std::cout << "GAMECONTROLLER DEBUG: run() started\n";
        
        if (!gameSession) {
            std::cout << "GAMECONTROLLER DEBUG: Game session is null!\n";
            std::cout << "Error: Game session not initialized!\n";
            return;
        }
        
        running = true;
        std::cout << "GAMECONTROLLER DEBUG: Showing help\n";
        visualizer.showHelp();
        
        std::cout << "GAMECONTROLLER DEBUG: Entering main game loop\n";
        while (running && !gameSession->isGameOver()) {
            std::cout << "GAMECONTROLLER DEBUG: --- New iteration of game loop ---\n";
            
            updateVisualization();
            
            if (gameSession->isPlayerTurn()) {
                std::cout << "GAMECONTROLLER DEBUG: Player's turn\n";
                visualizer.showPlayerTurn(gameSession->getHero());
                showSpellInfo();  
                
                std::cout << "GAMECONTROLLER DEBUG: Getting player command\n";
                PlayerCommand cmd = commandInput.getCommand();
                
                std::cout << "GAMECONTROLLER DEBUG: Processing player command\n";
                if (!processCommand(cmd)) {
                    std::cout << "GAMECONTROLLER DEBUG: processCommand returned false, stopping\n";
                    running = false;
                }
            } else {
                std::cout << "GAMECONTROLLER DEBUG: Enemy's turn\n";
                visualizer.showMessage("Enemy turn...");
                gameSession->processEnemyTurn();
                gameSession->checkCollisions();
                gameSession->endPlayerTurn();
            }
            
            if (gameSession->isGameOver()) {
                std::cout << "GAMECONTROLLER DEBUG: Game over condition met\n";
                if (gameSession->isVictory()) {
                    std::cout << "GAMECONTROLLER DEBUG: Victory!\n";
                    visualizer.showVictoryScreen();
                } else {
                    std::cout << "GAMECONTROLLER DEBUG: Defeat!\n";
                    visualizer.showDefeatScreen();
                }
                running = false;
            }
        }
        
        std::cout << "GAMECONTROLLER DEBUG: Game loop ended\n";
        updateVisualization();
        
        if (gameSession->isGameOver()) {
            std::cout << "\nGame over! Press Enter to continue...\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
        
        std::cout << "GAMECONTROLLER DEBUG: run() completed\n";
    }
    
    void stop() {
        std::cout << "GAMECONTROLLER DEBUG: stop() called\n";
        running = false;
    }
    
    bool isRunning() const { return running; }
    GameSession* getGameSession() const { return gameSession; }
};

#endif