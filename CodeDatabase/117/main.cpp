#include "Game.h"
#include "LogSystem/Notifier.h"
#include "LogSystem/LogObserver.h"

int main(int argc, char* argv[]){
    LogType loggingType = LogType::Console;
    std::string logFilename = DEFAULT_LOGFILE_NAME;
    for (int i = 1; i < argc; ++i){
        std::string arg = argv[i];
        if(arg == "-f" or arg == "--fileLogging"){
            loggingType = LogType::InFile;
            if (i < argc - 1){
                logFilename = argv[++i];
            }
        }
        else if (arg == "-c" or arg == "--consoleLogging"){
            loggingType = LogType::Console;
        }
    }
    try{
        std::shared_ptr<LogObserver> logger = std::make_shared<LogObserver>(loggingType, logFilename);
        Notifier::getInstance().subscribe(logger);
    }
    catch(const std::exception& e){
        std::cerr << "Log Error: " << e.what() << std::endl;
        return 1;
    }

    Game game;
    game.run();
    return 0;
}