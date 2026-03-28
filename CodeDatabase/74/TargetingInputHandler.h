#pragma once
#include "InputHandler.h"
#include "Game.h"
#include "Logger.h"
#include <cctype>

class TargetingInputHandler : public InputHandler {
public:
    void handle(Game& game, Command cmd) override {
        auto& player = game.getPlayer();
        auto& field = game.getField();
        auto& enemies = game.getEnemies();
        auto& aimPos = game.getAimPos();
        std::string message;
        bool actionPerformed = false;

        switch (cmd.type) {
            case CommandType::MoveUp: {
                if (aimPos.y > 0) aimPos.y--;
                break;
            }
            case CommandType::MoveDown: {
                if (aimPos.y < field.getHeight() - 1) aimPos.y++;
                break;
            }
            case CommandType::MoveLeft: {
                if (aimPos.x > 0) aimPos.x--;
                break;
            }
            case CommandType::MoveRight: {
                if (aimPos.x < field.getWidth() - 1) aimPos.x++;
                break;
            }
            case CommandType::SwitchMode: {
                game.getActionMode() = PlayerActionMode::Normal;
                message = "Ranged attack cancelled.";
                Logger::log("Ranged attack cancelled.");
                break;
            }
            case CommandType::Action: {
                for (auto it = enemies.begin(); it != enemies.end();) {
                if (it->getCoords().x == aimPos.x && it->getCoords().y == aimPos.y && it->isAlive()) {
                    it->takeDamage(player.getDamage());
                    message = "Ranged attack hit!";
                    Logger::log("Ranged attack hit!");
                    actionPerformed = true;
                    if (!it->isAlive()) {
                        it = enemies.erase(it);
                        player.getHand().generateSpell();
                    }
                    else ++it;
                    break;
                }
                else ++it;
                }

                if (!actionPerformed) {
                    message = "Ranged attack missed.";
                    Logger::log("Ranged attack missed.");
                    actionPerformed = true;
                    game.setAim(false, {0,0}, false, 0, false);
                }
                game.getActionMode() = PlayerActionMode::Normal;
                game.setAim(false, {0,0}, false, 0, false);
                break;
            }
            case CommandType::Save: {
                game.save();
                game.setCurrentMessage("Game saved!");
                break;
            }
        }

        game.setCurrentMessage(message);
        game.setAim(true, aimPos, false, 0, false);
        if (actionPerformed) game.switchTurn();
    }
};