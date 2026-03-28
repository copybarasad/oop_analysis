#pragma once
#include "InputHandler.h"
#include "Game.h"
#include "Logger.h"
#include <cctype>

class SplashTargetingInputHandler : public InputHandler {
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
                game.applySplashSpell(spellPtr, aimPos);
                game.getActionMode() = PlayerActionMode::Normal;
                actionPerformed = true;
                break;
            }
            case CommandType::SwitchMode: {
                game.getActionMode() = PlayerActionMode::Normal;
                message = "Splash cancelled.";
                Logger::log("Splash cancelled.");
                break;
            }
            case CommandType::Save: {
                game.save();
                game.setCurrentMessage("Game saved!");
                break;
            }
            
        }

        int radius = 0;
        auto spellPtr = player.getHand().getSpell(currentSpellIndex);
        if (spellPtr) radius = spellPtr->getCastInfo().radius;

        game.setCurrentMessage(message.empty() ? "Splash targeting: WASD to move, E to cast, Q to cancel." : message);
        game.setAim(true, aimPos, true, radius, false);

        if (actionPerformed) game.switchTurn();
    }
};
