#include "GameLogic.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "GameTemplates.h"
#include "Logger.h"
#include <string>
#include <memory>
#include <optional>

int main(int argc, char* argv[]) {
    bool useFileLog = false;
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--log-file") useFileLog = true;
    }

    if (useFileLog) {
        EventManager::getInstance().addLogger(std::make_shared<FileLogger>("game_log.txt"));
    } else {
        EventManager::getInstance().addLogger(std::make_shared<ConsoleLogger>());
    }

    EventManager::getInstance().notify(EventType::GameFlow, "System Started");

    GameLogic game;

    GameVisualizer<SfmlRenderer> visualizer;

    GameController<SfmlInputHandler> controller(game);

    controller.getInput().loadConfig("keys.cfg");

    auto& window = visualizer.getRenderer().getWindow();
    controller.getInput().setWindow(window);

    while (visualizer.isOpen() && game.isRunning()) {

        while (const std::optional<sf::Event> event = visualizer.getRenderer().pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                visualizer.getRenderer().close();
            }

            controller.handleEvent(*event);
        }

        game.update();
        visualizer.draw(game, controller.getSelectedSpellIndex());
    }

    EventManager::getInstance().notify(EventType::GameFlow, "System Shutdown");
    return 0;
}