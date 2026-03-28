#include <iostream>
#include <chrono>
#include <random>

#ifdef _WIN32
#include <windows.h>
#endif

#include "game.h"
#include "gameconfig.h"
#include "game_controller.h"
#include "keyboard_input.h"
#include "renderer.h"
#include "visualizer.h"

int main() {
    // Game configuration
    game::GameConfig cfg;

    // Initialize random number generator
    unsigned seed = static_cast<unsigned>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    std::mt19937 rng(seed);

    // Create game, gamecontroller, visualizer
    game::Game game(cfg, rng);
    
    game::GameController<game::KeyboardInput> controller;
    game::Visualizer<game::ConsoleRenderer> visual;

    while (!game.exitRequested()) {
        visual.draw(game);
        controller.step(game);
    }

    visual.clear();

    return 0;
}