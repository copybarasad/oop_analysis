#pragma once
#include "IRenderer.h"
#include <iostream>

template <typename Policy>
class RendererTemplate : public IRenderer {
public:
    void renderGame(const Game& game) override {
        Policy::render(game);
    }

    void renderMenu() override {
        std::cout << "\n----- ГЛАВНОЕ МЕНЮ -----\n";
    }
};
