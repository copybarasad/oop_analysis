#ifndef RPG_APPLICATION_H
#define RPG_APPLICATION_H

#include "GameController.h"
#include "GameVisualizer.h"

template <typename InputManager, typename Renderer>
class Application {
    bool running;
    std::shared_ptr<Game> game;
    GameController<InputManager> controller;
    GameVisualizer<Renderer> visualizer;
public:
    Application() : running(true), game(nullptr), controller(running, game), visualizer(game) {}

    void run() {
        visualizer.visualize(controller.getControls(), false);
        while (running) {
            auto [alert, message] = controller.executeCommands();
            if (!game)
                message = message.empty() ? controller.getControls() : message;
            visualizer.visualize(message, alert);
        }
    }
};

#endif
