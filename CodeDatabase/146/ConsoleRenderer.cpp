#include "ConsoleRenderer.h"
#include "Game.h"
#include "GameArea.h" 
#include "Player.h"
#include <iostream>
#include <vector>

void ConsoleRenderer::render(Game& game) {

    GameArea* area = game.get_area();
    Player* player = game.get_player();

    if (!area || !player) return;

    std::vector<std::string> lines = area->to_lines();
    for (const auto& line : lines) {
        std::cout << line << "\n";
    }

    std::cout << "\n=== Status ===\n";
    std::cout << "HP: " << player->get_health()
        << " | EXP: " << player->get_exp() << "\n";
    std::cout << "Spells: " << player->get_hand()->print() << "\n";
}
