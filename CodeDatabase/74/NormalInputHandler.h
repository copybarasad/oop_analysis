#pragma once
#include "InputHandler.h"
#include "Command.h"
#include "Game.h"
#include "Logger.h"
#include "Input.h"
#include <cctype>

class NormalInputHandler : public InputHandler {
public:
    void handle(Game& game, Command cmd) override {
        auto& player = game.getPlayer();
        auto& field = game.getField();
        auto& enemies = game.getEnemies();
        auto& aimPos = game.getAimPos();
        std::string message;
        bool actionPerformed = false;

        int px = player.getCoords().x;
        int py = player.getCoords().y;

        switch (cmd.type) {
        case CommandType::MoveUp:
            if (field.isCellWalkable(px, py - 1) && !game.isCellOccupiedByEnemy(px, py - 1)) {
                player.move(0, -1);
                message = "Moved up.";
                Logger::log("Moved up.");
                actionPerformed = true;
            }
            break;
        case CommandType::MoveDown:
            if (field.isCellWalkable(px, py + 1) && !game.isCellOccupiedByEnemy(px, py + 1)) {
                player.move(0, 1);
                message = "Moved down.";
                Logger::log("Moved down.");
                actionPerformed = true;
            }
            break;
        case CommandType::MoveLeft:
            if (field.isCellWalkable(px - 1, py) && !game.isCellOccupiedByEnemy(px - 1, py)) {
                player.move(-1, 0);
                message = "Moved left.";
                Logger::log("Moved left.");
                actionPerformed = true;
            }
            break;
        case CommandType::MoveRight:
            if (field.isCellWalkable(px + 1, py) && !game.isCellOccupiedByEnemy(px + 1, py)) {
                player.move(1, 0);
                message = "Moved right.";
                Logger::log("Moved right.");
                actionPerformed = true;
            }
            break;

        case CommandType::Action:
            if (player.getAttackMode() == AttackMode::Melee) {
                // --- MELEE ATTACK ---
                for (int y = py - 1; y <= py + 1; ++y) {
                    for (int x = px - 1; x <= px + 1; ++x) {
                        for (auto it = enemies.begin(); it != enemies.end();) {
                            if (it->getCoords().x == x && it->getCoords().y == y && it->isAlive()) {
                                it->takeDamage(player.getDamage());
                                message = "Melee hit!";
                                actionPerformed = true;
                                if (!it->isAlive()) {
                                    it = enemies.erase(it);
                                    player.getHand().generateSpell();
                                }
                                else ++it;
                            }
                            else ++it;
                        }
                    }
                }
            }
            else {
                // --- RANGED ATTACK ---
                game.getActionMode() = PlayerActionMode::Targeting;
                aimPos = player.getCoords();
                game.setAim(true, aimPos, false, 0, false);
                game.setCurrentMessage("Select target (WASD + E).");
                return;
            }
            break;

        case CommandType::SwitchMode: {
            AttackMode newMode = (player.getAttackMode() == AttackMode::Melee)
                ? AttackMode::Ranged : AttackMode::Melee;
            player.switchAttackMode(newMode);
            message = (newMode == AttackMode::Melee) ? "Switched to Melee." : "Switched to Ranged.";
            Logger::log("Switched attack mode.");
            break; // ����� ������ �� ������ ���
        }

        case CommandType::OpenHand: {
            game.setShowHand(true);
            game.setCurrentMessage("Select spell number or [Q] to cancel.");
            break;
        }

        case CommandType::SelectSpell: {
            if (game.getShowHand()) {
                int index = cmd.value;
                auto& hand = player.getHand();
                if (index >= 0 && index < hand.getCount()) {
                    game.castSpell(index);
                    actionPerformed = true;
                } else {
                    message = "Invalid spell index.";
                }
                game.setShowHand(false);
            } else {
                message = "Press R to open hand first.";
            }
            break;
        }

        case CommandType::Save:
            game.save();
            game.setCurrentMessage("Game saved!");
            return;

        case CommandType::Quit:
            if (game.getShowHand()) {
                game.setShowHand(false);
                game.setCurrentMessage("Cancelled spell selection.");
                return;
            } else {
                game.stop();
                return;
            }

        default:
            message = "Unknown command!";
            break;
        }

        game.setCurrentMessage(message);
        game.setAim(false, aimPos, false, 0, false);
        if (actionPerformed) game.switchTurn();
    }
};
