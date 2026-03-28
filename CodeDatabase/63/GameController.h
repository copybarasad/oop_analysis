#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Game.h"
#include "Player.h"
#include "Commands/Command.h"

template <typename InputHandler>
class GameController {
    InputHandler input;
    bool& running;
    std::shared_ptr<Game>& game;
    bool changeMode;
public:
    GameController(bool& running, std::shared_ptr<Game>& game) : running(running), game(game), changeMode(false) {
        input.setupMenuControls(running, game, changeMode);
    }

    std::pair<bool, std::string> executeCommands() {
        if (changeMode) {
            input.getCommand();
            changeMode = false;
            std::string message;
            if (game)
                message = input.setupGameControls(game, changeMode);
            else
                message = input.setupMenuControls(running, game, changeMode);
            return {!message.empty(), message};
        }
        Command* cmd = nullptr;
        while (!cmd)
            cmd = input.getCommand();
        auto [runCycle, message] = cmd->execute();
        if (game) {
            auto& player = game->getPlayer();
            if (runCycle) {
                game->gameCycle();
                if (auto levelStatus = game->isLevelEnded()) {
                    changeMode = true;
                    if (levelStatus == LOSE) {
                        game = nullptr;
                        return {true, "Поражение! Игрок погиб!\nНажмите любую клавишу для возврата в меню\n"};
                    }
                    return {true, "Уровень пройден!\nНажмите любую клавишу для продолжения\n"};
                }
                if (player.isSlowed()) {
                    player.setSlowed(false);
                    game->gameCycle();
                    message = "Вы наступили в замедляющую клетку! Все сделали 2 хода!\n";
                    if (auto levelStatus = game->isLevelEnded()) {
                        changeMode = true;
                        if (levelStatus == LOSE) {
                            game = nullptr;
                            return {true, message + "Поражение! Игрок погиб!\nНажмите любую клавишу для возврата в меню\n"};
                        }
                        return {true, message + "Уровень пройден!\nНажмите любую клавишу для продолжения\n"};
                    }
                    return {false, message};
                }
            }
            if (player.getMode() == PlayerModes::UPGRADE && !player.hasUpgradePoints())
                game->nextLevel();
        }
        return {changeMode, message};
    }

    std::string getControls() { return input.getControls(); }
};

#endif
