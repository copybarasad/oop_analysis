#include <iostream>
#include <cstdlib>
#include <ctime>
#include "game.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    srand(time(nullptr));

    if (argc >= 2 && std::string(argv[1]) == "--log-file") {
        std::string filename = "game.log";
        if (argc >= 3) filename = argv[2];
        global_logger() = std::make_shared<FileLogger>(filename);
    } else {
        global_logger() = std::make_shared<ConsoleLogger>();
    }
    
    Game game;
    game.run();
    
    return 0;
}