#ifndef RENDERER_H
#define RENDERER_H

#include "renderer_interface.h"
#include <iostream>

class Renderer : public IRenderer {
public:
    virtual void render(Game* game,
                       AppState state,
                       int cursorX,
                       int cursorY,
                       int spellIndex) override {

        if (state == AppState::MENU) {
            drawMenu();
            return;
        }

        if (!game->isInGame())
            return;

        GameField* field = game->getFieldPtr();
        Player* player = game->getPlayerPtr();

        if (!field || !player)
            return;
        
        field->display(*player, game->getEnemies());
        drawHUD(*player);
        drawControls();

        if (state == AppState::WIN) {
            std::cout << "      WIN!       \n";
            std::cout << "Press any key to return to menu...\n";
            return;
        }

    }

private:
    void drawMenu() {
        std::cout << "=== MAGIC GAME ===\n";
        std::cout << "1. New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Exit\n";
    }

    void drawHUD(const Player& player) {
        std::cout << "\nHP: " << player.getHealth()
                  << " | Spell: " << player.getCurrentSpell()
                  << "\n";
    }

    void drawControls() {
        std::cout << "\nControls:\n";
        std::cout << "WASD - Move\n";
        std::cout << "F    - Cast spell\n";
        std::cout << "C    - Change spell\n";
        std::cout << "Q    - Back to menu\n";
    }
};

#endif