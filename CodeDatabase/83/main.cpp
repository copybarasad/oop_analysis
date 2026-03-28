#include "game.h"
#include "controller.h"
#include "input_handler.h"
#include "renderer.h"
#include "visualizer.h" 
#include "event_dispatcher.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    std::ios_base::sync_with_stdio(false);
    Game game;
    EventDispatcher dispatcher;

    Logger logger(
        dispatcher,
        (argc > 1 && std::string(argv[1]) == "file")
            ? Logger::Mode::FILE
            : Logger::Mode::CONSOLE,
        "game.log"
    );

    game.setEventDispatcher(&dispatcher);

    InputHandler input("controls.cfg");
    
    Renderer renderer;
    
    Visualizer<Renderer> visualizer(renderer);
    
    Controller<InputHandler> controller(visualizer);

    controller.run(game);
    return 0;
}