#include "ConsoleGameRenderer.h"
#include "../Game.h"
#include "../Player.h"
#include "../GameField.h"
#include "../Enemy.h"
#include "../Towers/ITower.h"
#include <iostream>
#include <iomanip>

void ConsoleGameRenderer::render(const Game& game) {
    const Player& player = game.getPlayer();
    std::cout << "Player: " << player.getName()
              << " | HP: " << player.getHealth() << "/" << player.getMaxHealth()
              << " | Mana: " << player.getMana() << "/" << player.getMaxMana()
              << " | Combat: " << player.getCombatTypeString()
              << " | Damage: " << player.getDamage()
              << " | Score: " << player.getScore()
              << " | Kills: " << player.getEnemiesDefeated()
              << std::endl;
    
    if (player.getSpellHand().getSpellCount() > 0) {
        player.getSpellHand().displayHand();
    }

    const GameField& field = game.getField();

    std::cout << "\nGame Field:" << std::endl;

    int width = field.getWidth();
    int height = field.getHeight();

    std::cout << " ";
    for (int x = 0; x < width; ++x) {
        std::cout << std::setw(2) << (x % 10);
    }
    std::cout << std::endl;

    for (int y = 0; y < height; ++y) {
        std::cout << (y % 10);

        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            char cellChar = '.';

            if (pos == player.getPosition()) {
                cellChar = 'P';
            } else {
                const Enemy* enemy_at_pos = game.findEnemyAtPosition(pos);
                if (enemy_at_pos) {
                    cellChar = 'E';
                } else if (game.hasTowerAtPosition(pos)) {
                    cellChar = 'T';
                } else if (game.hasTrapAtPosition(pos)) {
                    cellChar = '*';
                } else if (!field.isCellPassable(pos)) {
                    cellChar = '#';
                }
            }

            std::cout << std::setw(2) << cellChar;
        }
        std::cout << std::endl;
    }
}