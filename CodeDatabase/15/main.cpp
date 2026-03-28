#include <iostream>
#include <string>
#include "Game.h"
#include "InputReader.h"
#include "Renderer.h"
#include "GameController.h"
#include "Visualizer.h"
#include "Logger.h"
#include "Exceptions.h"

int main(int argc, char* argv[]) {
    try {
        constexpr int kFieldWidth = 20;
        constexpr int kFieldHeight = 15;
        
        Game game(kFieldWidth, kFieldHeight);

        std::unique_ptr<ILogger> debugLogger;
        bool logToFile = true;
        
        if (argc > 1 && std::string(argv[1]) == "file") {
            logToFile = true;
        }

        if (logToFile) {
            debugLogger = std::make_unique<FileLogger>("game_log.txt");
            game.addObserver(debugLogger.get());
            std::cout << "Logging to game_log.txt" << std::endl;
        } 

        InputReader input_reader;
        input_reader.loadConfig("controls.cfg");
        
        Renderer concrete_renderer;
        
        using VisType = GameVisualizer<Renderer>;
        VisType visualizer(game, concrete_renderer);
        
        GameController<InputReader, VisType> controller(game, input_reader, visualizer);

        controller.run();

    } catch (const GameException& e) {
        std::cerr << "Fatal Game Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}