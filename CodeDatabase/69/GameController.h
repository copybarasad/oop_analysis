#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Game.h"
#include <iostream>

template <typename T_InputReader, typename T_View>
class GameController {
public:
    GameController(Game& game, T_InputReader& inputReader, T_View& view)
        : game_(game), inputReader_(inputReader), view_(view) {}

void Run() {
    bool shouldExit = false;
    
    while (!shouldExit) {
        view_.ShowMainMenu();
        int choice = inputReader_.ReadInt();

        if (choice == 1) {
            game_.StartNew(1);
            RunGame();
        } else if (choice == 2) {
            view_.ShowEnterFilenamePrompt();
            std::string filename = inputReader_.ReadString();
            try {
                game_.Load(filename);
                view_.ShowLoadSuccessMessage();
                RunGame();
            } catch (const std::exception& e) {
                view_.ShowLoadErrorMessage(e.what());
            }
        }
        else if (choice == 3) {
            view_.ShowExitMessage();
            shouldExit = true;  
        } else {
            view_.ShowInvalidChoiceMessage();
        }
    }
}

private:
    void RunGame() {
        if (!game_.GetPlayer()) {
            game_.StartNew(1);
        }

        while (!game_.IsGameOver()) {
            
            view_.Update(game_);

            auto command = inputReader_.ReadCommand();
            if (command) {
                auto result = command->execute(game_);
                view_.ShowActionMessage(result.message);
                
                if (result.success && command->EndsTurn()) {
                    game_.ProcessTurn();
                }
            } else {
                continue;
            }

            if (game_.IsGameOver()) break;

            Player* player = game_.GetPlayer();
            if (player && !player->IsAlive()) {
                view_.ShowGameOverPrompt();
                char ch = inputReader_.ReadChar();
                if (ch == 'y' || ch == 'Y') {
                    game_.StartNew(1);
                    continue; 
                } else {
                    view_.ShowExitMessage();
                    game_.SetGameOver(true);
                    break;
                }
            }

            if (game_.GetEnemies().empty() && game_.GetTowers().empty()) {
                int level = game_.GetLevel();
                view_.ShowLevelClearMessage(level);

                if (level >= 3) {
                    view_.ShowVictoryMessage();
                    game_.SetGameOver(true);
                    break;
                }

                view_.ShowUpgradeMenu();
                
                int up = inputReader_.ReadInt();
                game_.ApplyUpgrade(up);
                
                game_.StartNew(level + 1);
            }
        }
        if (game_.IsGameOver()) { 
            view_.ShowExitMessage();
        }
    }

    Game& game_;
    T_InputReader& inputReader_;
    T_View& view_;
};

#endif
