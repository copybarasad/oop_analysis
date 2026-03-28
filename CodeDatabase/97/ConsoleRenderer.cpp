#include "ConsoleRenderer.h"
#include "GameView.h"
#include "Player.h"
#include <iostream>

void ConsoleRenderer::draw(const GameState& state) {
    GameView gv;
    gv.render(state.model());

    auto p = state.player();
    std::cout << "----------------------------------------\n";
    if (p) {
        std::cout << "HP: " << p->hp()
                  << "   Mode: " << p->modeName()
                  << "   Score: " << p->score()
                  << "   Hand: " << p->hand().size() << "/" << p->hand().capacity()
                  << "\n";
    } else {
        std::cout << "Player: (missing)\n";
    }

    std::cout << "\n input";
}
