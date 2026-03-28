#pragma once
#include "InputHandler.h"
#include "Game.h"
#include "Logger.h"
#include <cctype>

class TrapTargetingInputHandler : public InputHandler {
public:
    void handle(Game& game, Command cmd) override {
        
        auto& player = game.getPlayer();
        auto& field = game.getField();
        auto& enemies = game.getEnemies();
        auto& aimPos = game.getAimPos();
        int& currentSpellIndex = game.getCurrentSpellIndex();
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
            case CommandType::Action: {
                auto spellPtr = player.getHand().getSpell(currentSpellIndex);
                
                game.placeTrap(spellPtr, aimPos, currentSpellIndex);
                game.getActionMode() = PlayerActionMode::Normal;
                actionPerformed = true;
                break;
            }
            case CommandType::SwitchMode: {
                game.getActionMode() = PlayerActionMode::Normal;
                message = "Trap placement cancelled.";
                Logger::log("Trap placement cancelled.");
                break;
            }
            case CommandType::Save: {
                game.save();
                game.setCurrentMessage("Game saved!");
                break;
            }
        }

        game.setCurrentMessage(message.empty() ? "Trap targeting: WASD to move, E to place, Q to cancel." : message);
        game.setAim(true, aimPos, false, 0, true);

        if (actionPerformed) game.switchTurn();
    }
};
