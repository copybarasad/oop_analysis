#include "game_manager.h"
#include "game_exceptions.h"
#include "event_system.h"
#include <iostream>

int main(int argc, char** argv) {
    try {
        // Configure loggers via startup parameter, e.g. --log=console or --log=file:game.log or both (--log=console,file:game.log)
        bool loggerAdded = false;
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (arg.rfind("--log=", 0) == 0) {
                std::string val = arg.substr(6);
                size_t start = 0;
                while (start < val.size()) {
                    size_t pos = val.find(',', start);
                    std::string token = (pos == std::string::npos) ? val.substr(start) : val.substr(start, pos - start);
                    if (token == "console") {
                        EventSystem::getInstance().addLogger(new ConsoleLogger());
                        loggerAdded = true;
                    } else if (token.rfind("file:", 0) == 0 || token.rfind("file=", 0) == 0) {
                        size_t idx = token.find(':');
                        if (idx == std::string::npos) idx = token.find('=');
                        std::string filename = token.substr(idx+1);
                        if (!filename.empty()) {
                            EventSystem::getInstance().addLogger(new FileLogger(filename));
                            loggerAdded = true;
                        }
                    }
                    if (pos == std::string::npos) break;
                    start = pos + 1;
                }
            }
        }
        if (!loggerAdded) {
            // default to console logger
            EventSystem::getInstance().addLogger(new ConsoleLogger());
        }

        GameManager gameManager;
        gameManager.run();
    } catch (const GameException& e) {
        std::cerr << "❌ КРИТИЧЕСКАЯ ОШИБКА: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "❌ НЕОЖИДАННАЯ ОШИБКА: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
