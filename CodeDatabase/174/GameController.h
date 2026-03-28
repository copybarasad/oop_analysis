#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Game.h"
#include "GameMenu.h"
#include "Command.h"
#include "CombatManager.h"
#include "Position.h"
#include "Constants.h"
#include <memory>
#include <iostream>

// ШАБЛОННЫЙ класс - параметр InputHandlerType может быть ЛЮБЫМ классом
template <typename InputHandlerType>
class GameController {
public:
    // Конструктор СОЗДАЕТ объект класса из параметра шаблона
    GameController(Game& game, std::unique_ptr<InputHandlerType> inputHandler)
        : game_(game), inputHandler_(std::move(inputHandler)), combatManager_(), 
          shouldReturnToMenu_(false) {}
    
    bool processNextCommand() {
        auto command = inputHandler_->getNextCommand();
        
        if (!command) {
            return false;
        }
        
        executeCommand(*command);
        
        return !shouldReturnToMenu_;
    }
        
    bool shouldReturnToMenu() const {
        return shouldReturnToMenu_;
    }
    
    void resetReturnFlag() {
        shouldReturnToMenu_ = false;
    }
    
private:
    Game& game_;
    std::unique_ptr<InputHandlerType> inputHandler_;
    CombatManager combatManager_;
    bool shouldReturnToMenu_;
    
    void executeCommand(const Command& command) {
        switch (command.getType()) {
            case CommandType::MOVE_UP:
                game_.movePlayer(Direction::UP);
                break;
            case CommandType::MOVE_DOWN:
                game_.movePlayer(Direction::DOWN);
                break;
            case CommandType::MOVE_LEFT:
                game_.movePlayer(Direction::LEFT);
                break;
            case CommandType::MOVE_RIGHT:
                game_.movePlayer(Direction::RIGHT);
                break;
            case CommandType::ATTACK:
                game_.performAttack();
                break;
            case CommandType::CAST_SPELL:
                game_.processPlayerSpell();
                break;
            case CommandType::CHANGE_STYLE:
                game_.changeFightingStyle();
                break;
            case CommandType::SAVE_GAME:
                handleSaveGame();
                break;
            case CommandType::LOAD_GAME:
                handleLoadGame();
                break;
            case CommandType::QUIT:
                handleQuitToMenu();
                break;
            case CommandType::UNKNOWN:
                std::cout << "Unknown command!" << std::endl;
                break;
        }
        
        if (!shouldReturnToMenu_) {
            updateGameAfterCommand();
        }
    }
    
    void handleSaveGame() {
        std::string filename = GameMenu::showSaveGameMenu();
        if (!filename.empty()) {
            game_.saveGame(filename);
        }
    }
    
    void handleLoadGame() {
        std::string filename = GameMenu::showLoadGameMenu();
        if (!filename.empty()) {
            game_.loadGame(filename);
        }
    }
    
    void handleQuitToMenu() {
        shouldReturnToMenu_ = true;
    }
    
    void updateGameAfterCommand() {
        game_.updateEnemies();
        game_.updateBarracks();
        game_.updateTowers();
        game_.updateAllies();
        game_.removeDeadUnits();
        
        game_.updateGameState();
        
        if (game_.getEnemies().empty() && game_.getBuildings().empty()) {
            game_.advanceToNextLevel();
        }
        
        if (game_.checkGameEndCondition()) {
            game_.quitGame();
        }
    }
};
#endif