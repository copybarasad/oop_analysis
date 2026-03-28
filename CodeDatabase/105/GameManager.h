#pragma once
#include "Command.h"
#include "Game.h"
#include "SpellCasting.h"
#include "GameVisualize.h"
#include "DirectDamageSpell.h"  
#include "AreaDamageSpell.h"  
#include <functional>
#include <map>
#include <iostream>
#include <memory>  

enum class GameState {
    MAIN_MENU,
    SIZE_INPUT,
    MAIN_GAME,
    IN_MENU,
    SPELL_SELECTION,
    SPELL_SHOP_SELECTION,
    COORDINATES_INPUT,
    CONFIRMATION,
    GAME_OVER,
    EXIT
};

template<typename InputHandlerType>
class GameManager {
private:
    InputHandlerType inputHandler;
    Game* game;
    GameState currentState;
    int selectedSpellIndex;
    ConsoleGameVisualizer visualizer; 
    
    std::map<GameState, std::function<void(Command)>> stateHandlers;
    
    void setupDefaultHandlers() {
        stateHandlers[GameState::MAIN_MENU] = [this](Command cmd) { handleMainMenuCommand(cmd); };
        stateHandlers[GameState::SIZE_INPUT] = [this](Command cmd) { handleSizeCommand(cmd); };
        stateHandlers[GameState::MAIN_GAME] = [this](Command cmd) { handleGameCommand(cmd); };
        stateHandlers[GameState::IN_MENU] = [this](Command cmd) { handleMenuCommand(cmd); };
        stateHandlers[GameState::SPELL_SELECTION] = [this](Command cmd) { handleSpellCommand(cmd); };
        stateHandlers[GameState::SPELL_SHOP_SELECTION] = [this](Command cmd) { handleSpellShopCommand(cmd); };
        stateHandlers[GameState::COORDINATES_INPUT] = [this](Command cmd) { handleCoordinatesCommand(cmd); };
        stateHandlers[GameState::CONFIRMATION] = [this](Command cmd) { handleConfirmation(cmd); };
        stateHandlers[GameState::GAME_OVER] = [this](Command cmd) { handleGameOverCommand(cmd); };
    }
    
    Command getCommandForCurrentState() {
        switch (currentState) {
            case GameState::MAIN_GAME:
                return inputHandler.getGameCommand();
            case GameState::IN_MENU:
            case GameState::GAME_OVER:
            case GameState::MAIN_MENU:
                return inputHandler.getMenuCommand();
            case GameState::SPELL_SELECTION:
                return inputHandler.getSpellCommand(game->getPlayer().getSpellHand().getSpellCount());
            case GameState::SPELL_SHOP_SELECTION:
                return inputHandler.getMenuCommand();
            case GameState::COORDINATES_INPUT:
                return inputHandler.getCoordinatesCommand();
            case GameState::SIZE_INPUT:
                return inputHandler.getSizeCommand();
            case GameState::CONFIRMATION:
                return inputHandler.getConfirmation() ? 
                       Command(CommandType::CONFIRM) : 
                       Command(CommandType::CANCEL);
            default:
                return Command(CommandType::INVALID);
        }
    }

public:
    GameManager(Game* gameObj, const std::string& configFile = "controls.cfg")
        : inputHandler(configFile), game(gameObj), currentState(GameState::MAIN_MENU),
          selectedSpellIndex(-1) {

         // статус конфига
        auto configStatus = inputHandler.getConfigStatus();
        if (!configStatus.isValid) {
            for (const auto& error : configStatus.errors) {
                visualizer.visualizeError("Config Error", error);
            }
            for (const auto& warning : configStatus.warnings) {
                visualizer.visualizeOperationResult("Config Warning", warning, "");
            }
        }

        setupDefaultHandlers();
        game->showSaveInfo();
    }
    
    void run() {
        while (currentState != GameState::EXIT) {
            processFrame();
        }
    }
    
    void processFrame() {
        if (currentState == GameState::EXIT) return;
        
        // Показываем подсказку для текущего состояния
        showStatePrompt();
        
        Command cmd = getCommandForCurrentState();
        
        auto handlerIt = stateHandlers.find(currentState);
        if (handlerIt != stateHandlers.end()) {
            handlerIt->second(cmd);
        }
    }
    
    void showStatePrompt() {
        switch (currentState) {
            case GameState::MAIN_MENU:
                visualizer.visualizeMainMenu();
                break;
            case GameState::SIZE_INPUT:
                visualizer.visualizeSizeInputPrompt();
                break;
            case GameState::MAIN_GAME:
                game->display();
                visualizer.visualizeControlsHelp(inputHandler.getControlsHelp());
                break;
            case GameState::IN_MENU:
                visualizer.visualizeExtendedMenu();
                break;
            case GameState::SPELL_SELECTION:
                SpellCasting::displaySpellInfo(game->getPlayer());
                visualizer.visualizeSpellSelectionPrompt(
                    game->getPlayer().getSpellHand().getSpellCount());
                break;
            case GameState::SPELL_SHOP_SELECTION:
                game->buySpell();
                break;
            case GameState::COORDINATES_INPUT:
                visualizer.visualizeCoordinatesInputPrompt();
                break;
            case GameState::CONFIRMATION:
                visualizer.visualizeConfirmationPrompt("Are you sure you want to quit?");
                break;
            case GameState::GAME_OVER:
                visualizer.visualizeGameOverMenu();
                break;
        }
    }
    
    virtual void handleMainMenuCommand(Command cmd) {
        if (cmd.getType() == CommandType::SELECT_OPTION) {
            int option = cmd.getInt();
            switch (option) {
                case 1:
                    currentState = GameState::SIZE_INPUT;
                    break;
                case 2:
                    try {
                        game->loadGame();
                        currentState = GameState::MAIN_GAME;
                    } catch (const std::exception& e) {
                        visualizer.visualizeError("loading game", e.what());
                    }
                    break;
                case 3:
                    currentState = GameState::EXIT;
                    break;
            }
        }
    }
    
    virtual void handleGameCommand(Command cmd) {
        switch (cmd.getType()) {
            case CommandType::MOVE_UP:
                game->processPlayerMove('w');
                game->processEnemyTurns();
                game->play();
                break;
            case CommandType::MOVE_DOWN:
                game->processPlayerMove('s');
                game->processEnemyTurns();
                game->play();
                break;
            case CommandType::MOVE_LEFT:
                game->processPlayerMove('a');
                game->processEnemyTurns();
                game->play();
                break;
            case CommandType::MOVE_RIGHT:
                game->processPlayerMove('d');
                game->processEnemyTurns();
                game->play();
                break;
            case CommandType::CAST_SPELL:
                if (game->getPlayer().getSpellHand().getSpellCount() > 0) {
                    currentState = GameState::SPELL_SELECTION;
                } else {
                    visualizer.visualizeOperationResult("spell", "no_spells", "No spells available!");
                }
                break;
            case CommandType::SWITCH_COMBAT:
                game->switchCombatType();
                game->processEnemyTurns();
                game->play();
                break;
            case CommandType::BUY_SPELL:
                currentState = GameState::SPELL_SHOP_SELECTION;
                break;
            case CommandType::SAVE_GAME:
                game->saveGame();
                break;
            case CommandType::SHOW_MENU:
                currentState = GameState::IN_MENU;
                break;
            case CommandType::QUIT_GAME:
                currentState = GameState::CONFIRMATION;
                break;
            case CommandType::INVALID:
                visualizer.visualizeOperationResult("command", "invalid", "Unknown command");
                break;
        }
    }
    
    virtual void handleMenuCommand(Command cmd) {
        if (cmd.getType() == CommandType::SELECT_OPTION) {
            int option = cmd.getInt();
            switch (option) {
                case 1:
                    currentState = GameState::SIZE_INPUT;
                    break;
                case 2:
                    game->switchCombatType();
                    currentState = GameState::MAIN_GAME;
                    break;
                case 3:
                    currentState = GameState::SPELL_SHOP_SELECTION;
                    break;
                case 4:
                    game->saveGame();
                    currentState = GameState::MAIN_GAME;
                    break;
                case 5:
                    currentState = GameState::MAIN_GAME;
                    break;
            }
        } else if (cmd.getType() == CommandType::CANCEL) {
            currentState = GameState::MAIN_GAME;
        }
    }
    
    virtual void handleSpellCommand(Command cmd) {
        if (cmd.getType() == CommandType::CAST_SPELL) {
            auto spellData = cmd.getSpell();
            selectedSpellIndex = spellData.spellIndex;
            
            if (spellData.hasTarget) {
                game->castSpell(selectedSpellIndex, spellData.targetX, spellData.targetY);
                game->processEnemyTurns();
                game->play();
                currentState = GameState::MAIN_GAME;
            } else {
                currentState = GameState::COORDINATES_INPUT;
            }
        } else if (cmd.getType() == CommandType::CANCEL) {
            currentState = GameState::MAIN_GAME;
        }
    }
    
    virtual void handleSpellShopCommand(Command cmd) {
        if (cmd.getType() == CommandType::SELECT_OPTION) {
            int option = cmd.getInt();
            
            if (option >= 1 && option <= 2) {
                if (game->getPlayer().getScore() >= 50) {
                    std::unique_ptr<Spell> newSpell;
                    if (option == 1) {
                        newSpell = std::make_unique<DirectDamageSpell>(
                            30,  // damage
                            3,   // range
                            "Fireball",  // name
                            "A fiery projectile",  // description
                            20    // mana cost
                        );
                    } else {
                        newSpell = std::make_unique<AreaDamageSpell>(
                            25,  // damage
                            2,   // range
                            2,   // areaSize
                            "Firestorm",  // name
                            "An area of flames",  // description
                            30    // mana cost
                        );
                    }
                    
                    if (game->getPlayer().getSpellHand().addSpell(std::move(newSpell))) {
                        game->getPlayer().addScore(-50);
                        
                        std::string spellName = (option == 1 ? "Fireball" : "Firestorm");
                        std::string message = "Purchased " + spellName + "!";
                        visualizer.visualizeOperationResult("shop", "purchase", message);
                    } else {
                        visualizer.visualizeOperationResult("shop", "full", "Spell hand is full!");
                    }
                } else {
                    visualizer.visualizeOperationResult("shop", "no_points", "Not enough points!");
                }
            } else if (option == 0) {
                visualizer.visualizeOperationResult("shop", "cancelled", "Purchase cancelled");
            }
            
            currentState = GameState::MAIN_GAME;
        } else if (cmd.getType() == CommandType::CANCEL) {
            currentState = GameState::MAIN_GAME;
        }
    }
    
    virtual void handleCoordinatesCommand(Command cmd) {
        if (cmd.getType() == CommandType::ENTER_COORDINATES) {
            auto coords = cmd.getCoords();
            game->castSpell(selectedSpellIndex, coords.first, coords.second);
            game->processEnemyTurns();
            game->play();
            currentState = GameState::MAIN_GAME;
        } else if (cmd.getType() == CommandType::CANCEL) {
            currentState = GameState::MAIN_GAME;
        }
    }
    
    virtual void handleSizeCommand(Command cmd) {
        if (cmd.getType() == CommandType::ENTER_SIZE) {
            auto size = cmd.getSize();
            game->startNewGame(size.first, size.second);
            currentState = GameState::MAIN_GAME;
        } else if (cmd.getType() == CommandType::CANCEL) {
            currentState = GameState::MAIN_MENU;
        }
    }
    
    virtual void handleConfirmation(Command cmd) {
        if (cmd.getType() == CommandType::CONFIRM) {
            currentState = GameState::MAIN_MENU;
            game->setGameRunning(false);
        } else if (cmd.getType() == CommandType::CANCEL) {
            currentState = GameState::MAIN_GAME;
        }
    }
    
    virtual void handleGameOverCommand(Command cmd) {
        if (cmd.getType() == CommandType::SELECT_OPTION) {
            int option = cmd.getInt();
            switch (option) {
                case 1:
                    currentState = GameState::MAIN_MENU;
                    break;
                case 2:
                    currentState = GameState::EXIT;
                    break;
            }
        }
    }
};