#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "GameManager.h"
#include "Command.h"
#include "InputHandler.h"
#include <memory>
#include <functional>

template<typename InputHandlerType>
class GameController {
public:
    GameController(std::unique_ptr<GameManager> gameManager)
        : gameManager(std::move(gameManager))
        , inputHandler(std::make_unique<InputHandlerType>())
        , gameRunning(true) {}

    void processInput() {
        auto command = inputHandler->getCommand();
        if (!command) return;

        if (command->isMove()) {
            const auto& cmd = command->template get<MoveCommand>();
            if (gameManager) {
                gameManager->movePlayer(cmd.direction);
            }
        }
        else if (command->isAttack()) {
            const auto& cmd = command->template get<AttackCommand>();
            if (gameManager) {
                gameManager->playerAttack(cmd.direction);
            }
        }
        else if (command->isCastSpell()) {
            const auto& cmd = command->template get<CastSpellCommand>();
            if (gameManager) {
                gameManager->castPlayerSpell(cmd.spellIndex, cmd.direction);
            }
        }
        else if (command->isSwitchMode()) {
            if (gameManager && gameManager->getPlayer()) {
                gameManager->getPlayer()->switchCombatMode();
            }
        }
        else if (command->isViewSpells()) {
            // Renderer будет обрабатывать это
        }
        else if (command->isSaveGame()) {
            const auto& cmd = command->template get<SaveGameCommand>();
            // Устанавливаем флаги
            saveRequested = true;
            lastSaveCommand = cmd.filename;
        }
        else if (command->isLoadGame()) {
            const auto& cmd = command->template get<LoadGameCommand>();
            // Устанавливаем флаги
            loadRequested = true;
            lastLoadCommand = cmd.filename;
        }
        else if (command->isQuit()) {
            gameRunning = false;
        }
        else if (command->isRestartLevel()) {
            restartRequested = true;
        }
        else if (command->isNextLevel()) {
            nextLevelRequested = true;
        }
    }

    GameManager* getGameManager() { return gameManager.get(); }
    const GameManager* getGameManager() const { return gameManager.get(); }
    bool isGameRunning() const { return gameRunning; }
    void stopGame() { gameRunning = false; }
    
    InputHandlerType* getInputHandler() { return inputHandler.get(); }
    
    // Методы для запросов от GameSession
    bool isSaveRequested() const { return saveRequested; }
    std::string getSaveFilename() { 
        std::string filename = lastSaveCommand;
        saveRequested = false;
        lastSaveCommand.clear();
        return filename; 
    }
    
    bool isLoadRequested() const { return loadRequested; }
    std::string getLoadFilename() { 
        std::string filename = lastLoadCommand;
        loadRequested = false;
        lastLoadCommand.clear();
        return filename; 
    }
    
    bool isRestartRequested() const { return restartRequested; }
    void clearRestartRequest() { restartRequested = false; }
    
    bool isNextLevelRequested() const { return nextLevelRequested; }
    void clearNextLevelRequest() { nextLevelRequested = false; }
    
    void setMenuMode(bool isMenuMode) {
        if (inputHandler) {
            inputHandler->setMode(isMenuMode);
        }
    }

private:
    std::unique_ptr<GameManager> gameManager;
    std::unique_ptr<InputHandlerType> inputHandler;
    bool gameRunning;
    
    // Флаги запросов
    bool saveRequested = false;
    bool loadRequested = false;
    bool restartRequested = false;
    bool nextLevelRequested = false;
    std::string lastSaveCommand;
    std::string lastLoadCommand;
};

#endif