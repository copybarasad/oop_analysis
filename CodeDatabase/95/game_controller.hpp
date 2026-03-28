#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "sys/command.hpp"
#include "sys/input_handler.hpp"
#include "game.hpp"
#include "game_manager.hpp"
#include "save_manager.hpp"
#include <iostream>

template <typename InputHandler, typename RendererImpl>
class GameController {
private:
    InputHandler input;
    GameManager gameManager;
    SaveManager saveManager;
    Renderer<RendererImpl> gameRenderer;
    RendererImpl& uiRenderer; 
    bool shouldExit = false;

public:
    GameController() : uiRenderer(gameRenderer.getRenderer()) {}
    
    void startGame() {
        showMainMenu();
    }

private:
    void showMainMenu() {
        while (!shouldExit) {
            uiRenderer.renderMainMenu();
            
            std::unique_ptr<Command> cmd = input.getNextCommand(GameMode::MAIN_MENU);
            if (auto* menuCmd = dynamic_cast<MenuCommand*>(cmd.get())) {
                switch (menuCmd->action) {
                    case MenuAction::START_NEW_GAME:
                        newGame();
                        break;
                    case MenuAction::LOAD_GAME:
                        showSaveMenu();
                        break;
                    case MenuAction::EXIT:
                        uiRenderer.renderMessage("Выход из игры.");
                        shouldExit = true;
                        break;
                    default:
                        uiRenderer.renderError("Неверный выбор!");
                }
            }
        }
    }
    
    void newGame() {
        cleanupGame();
        
        Game* game = gameManager.createNewGame();
        if (!game) {
            uiRenderer.renderError("Ошибка создания игры!");
            return;
        }
        
        GameStatus result = game->run(input);
        handleGameResult(result);
    }
    
    void continueGame() {
        Game* game = gameManager.getCurrentGame();
        if (game) {
            game->getGameState().setStatus(GameStatus::Running);
            GameStatus result = game->run(input);
            handleGameResult(result);
        }
    }
    
    void handleGameResult(GameStatus result) {
        switch (result) {
            case GameStatus::Win:
                if (gameManager.getCurrentGame()->proceedToNextLevel()) {
                    continueGame();
                } else {
                    uiRenderer.renderMessage("Игра окончена. Вы победили!");
                    cleanupGame();
                }
                break;
            case GameStatus::Lose:
                uiRenderer.renderMessage("Игра окончена. Вы проиграли!");
                cleanupGame();
                break;
            case GameStatus::Paused:
                showPauseMenu();
                break;
            default:
                cleanupGame();
        }
    }
    
    void showPauseMenu() {
        while (gameManager.hasGame() && !shouldExit) {
            uiRenderer.renderPauseMenu();
            
            std::unique_ptr<Command> cmd = input.getNextCommand(GameMode::PAUSE_MENU);
            if (auto* menuCmd = dynamic_cast<MenuCommand*>(cmd.get())) {
                switch (menuCmd->action) {
                    case MenuAction::START_NEW_GAME:
                        continueGame();
                        break;
                    case MenuAction::LOAD_GAME:
                        showSaveMenu();
                        break;
                    case MenuAction::SAVE_GAME_NAME:
                        saveCurrentGame();
                        continueGame();
                        break;
                    case MenuAction::BACK:
                        saveCurrentGame();
                        cleanupGame();
                        showMainMenu();
                        return;
                    case MenuAction::EXIT:
                        cleanupGame();
                        showMainMenu();
                        return;
                    default:
                        uiRenderer.renderError("Неверный выбор!");
                }
            }
        }
    }
    
    void showSaveMenu() {
        auto saves = saveManager.getSaveList();
        
        if (saves.empty()) {
            uiRenderer.renderMessage("Нет сохраненных игр!");
            return;
        }
        
        uiRenderer.renderSaveMenu(saves);
        
        std::unique_ptr<Command> cmd = input.getNextCommand(GameMode::SAVE_MENU);
        if (auto* menuCmd = dynamic_cast<MenuCommand*>(cmd.get())) {
            if (menuCmd->action == MenuAction::BACK) {
                return;
            } else if (menuCmd->action == MenuAction::SELECT_SAVE && menuCmd->index) {
                int choice = menuCmd->index.value();
                
                if (choice > 0 && choice <= (int)saves.size()) {
                    loadGame(saves[choice - 1]);
                } else {
                    uiRenderer.renderError("Неверный выбор!");
                }
            } else {
                uiRenderer.renderError("Неверный выбор!");
            }
        }
    }
    
    void loadGame(const std::string& saveName) {
        cleanupGame();

        Game* game = gameManager.loadGameFromSave(saveName);
        if (!game) {
            uiRenderer.renderError("Ошибка создания игры для загрузки!");
            return;
        }
        
        if (saveManager.loadGame(game->getGameState(), saveName)) {
            uiRenderer.renderMessage("Игра загружена успешно!");
            continueGame();
        } else {
            uiRenderer.renderError("Ошибка загрузки игры!");
            cleanupGame();
        }
    }
    
    void saveCurrentGame() {
        Game* game = gameManager.getCurrentGame();
        if (game) {
            uiRenderer.renderSaveNamePrompt();      
            if (saveManager.saveGame(game->getGameState())) {
                uiRenderer.renderSaveSuccess();
            } else {
                uiRenderer.renderError("Ошибка сохранения игры!");
            }
        }
    }
    
    void cleanupGame() {
        gameManager.cleanupGame();
    }
};

#endif