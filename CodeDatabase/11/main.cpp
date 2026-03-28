#include <iostream>
#include <string>
#include "Field.h"
#include "Player.h"
#include "GameController.h"
#include "InputReader.h"
#include "ConsoleRenderer.h"
#include "GameEngine.h"
#include "EventLogger.h"

int main(int argc, char* argv[]) {
    try {
        LogOutput logType = LogOutput::BOTH;
        std::string logFilename = "game_log.txt";

        if (argc > 1) {
            std::string arg = argv[1];
            if (arg == "file") logType = LogOutput::FILE;
            else if (arg == "both") logType = LogOutput::BOTH;
        }
        EventLogger eventLogger(logType, logFilename);
        Field gameField(15, 15);
        Player player(7, 7, 100, 30, 20); 
        GameController gameController(player, gameField, eventLogger);
        GameEngine<InputReader, ConsoleRenderer> engine(gameController);
        engine.run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}