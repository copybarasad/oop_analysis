#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "game_controller.h"
#include <iostream>
#include <string>

class ConsoleRenderer {
public:
    void render(const GameController& game) const {
        const Player& player = game.getPlayer();
        const GameField& field = game.getField();
        const std::vector<Enemy>& enemies = game.getEnemies();
        const std::vector<EnemyTower>& towers = game.getTowers();

        std::cout << "\nGAME STATE\n";
        std::cout << "Player - HP: " << player.getHealth()
                  << " | Score: " << player.getScore()
                  << " | Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED")
                  << " | Range: " << player.getAttackRange() << "\n";

        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                Position currentPos(x, y);

                if (player.getPosition() == currentPos) {
                    std::cout << "P ";
                    continue;
                }

                bool rendered = false;
                for (const auto& enemy : enemies) {
                    if (enemy.isAlive() && enemy.getPosition() == currentPos) {
                        std::cout << "E ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                for (const auto& tower : towers) {
                    if (tower.getPosition() == currentPos) {
                        std::cout << "T ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                for (const auto& trap : field.getTraps()) {
                    if (trap.isActive() && trap.getPosition() == currentPos) {
                        std::cout << "^ ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                if (field.getCell(x, y).getType() == CellType::OBSTACLE) {
                    std::cout << "# ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }

        const PlayerHand& hand = player.getHand();
        std::cout << "\nSPELLS (" << hand.size() << "/" << hand.getMaxSize() << "):\n";
        if (hand.size() == 0) {
            std::cout << "  (none)\n";
        } else {
            for (size_t i = 0; i < hand.getSpells().size(); ++i) {
                const auto& spell = hand.getSpells()[i];
                std::cout << "  [" << (i + 1) << "] " << spell->getName()
                          << " - " << spell->getDescription() << "\n";
            }
        }

        renderControls();
    }

    void renderControls() const {
        std::cout << "\nCONTROLS: W/A/S/D (move), I/J/K/L (attack),\n"
                     "          1/2/3 (spell), B (buy), M (mode), Z (save), Q (quit)\n";
    }

    void renderMessage(const std::string& msg) const {
        std::cout << msg << std::endl;
    }

    void renderPlayerMoved() const {
        std::cout << "Player moved.\n";
    }

    void renderCannotMove() const {
        std::cout << "Cannot move there! Obstacle or out of bounds.\n";
    }

    void renderPlayerAttacked(int damage) const {
        std::cout << "Player attacked with " << damage << " damage!\n";
    }

    void renderAttackMissed() const {
        std::cout << "Attack missed! No enemy in range.\n";
    }

    void renderModeSwitched(CombatMode mode, int damage) const {
        std::cout << "Combat mode switched to " 
                  << (mode == CombatMode::MELEE ? "MELEE" : "RANGED")
                  << " (Damage: " << damage << ")\n";
    }

    void renderSpellPurchased() const {
        std::cout << "Spell purchased for 30 points!\n";
    }

    void renderNotEnoughScore() const {
        std::cout << "Not enough points (need 30)!\n";
    }

    void renderHandFull() const {
        std::cout << "Hand is full — cannot buy more spells.\n";
    }

    void renderSpellUsed() const {
        std::cout << "Spell used successfully!\n";
    }

    void renderSpellFailed() const {
        std::cout << "Spell failed: invalid target or out of range.\n";
    }

    void renderGameSaved(const std::string& filename) const {
        std::cout << "Game saved successfully to " << filename << "!\n";
    }

    void renderSaveFailed(const std::string& reason) const {
        std::cout << "Error saving game: " << reason << "\n";
    }

    void renderUnknownCommand() const {
        std::cout << "Unknown or invalid command.\n";
    }

    void renderPlayerDamage(int damage, int oldHP, int newHP) const {
        std::cout << "Player took " << damage << " damage! HP: " 
                  << oldHP << " -> " << newHP << "\n";
    }

    void renderEnemyDamage(int enemyId, int damage, int oldHP, int newHP, bool defeated) const {
        std::cout << "Enemy " << enemyId << " took " << damage << " damage! HP: "
                  << oldHP << " -> " << newHP;
        if (defeated) {
            std::cout << " - ENEMY DEFEATED! +30 points!";
        }
        std::cout << "\n";
    }

    void renderGameExited() const {
        std::cout << "Game exited by player.\n";
    }

    void renderVictory() const {
        std::cout << "\nVICTORY! Level completed!\n";
    }

    void renderGameOver() const {
        std::cout << "\nGAME OVER! You have been defeated.\n";
    }
};

#endif