#include <memory>
#include "gamemodel.h"
#include "gameview.h"
#include "inputhandler.h"
#include "gamecontroller.h"
#include "visualization.h"
#include "consolelogger.h"
#include "filelogger.h"
#include "ArgParser.h"
#include <QMainWindow>
#include <windows.h>


int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ArgParser parser(argc, argv);


    if (parser.hasFlag("--help")) {
        parser.printHelp();
        return 0;
    }


    if (!parser.hasFlag("--nolog")) {

        LogLevel level = LogLevel::INFO;
        std::string levelStr = parser.getOption("--loglevel", "info");

        if (levelStr == "debug") level = LogLevel::DEBUG;
        else if (levelStr == "warning") level = LogLevel::WARN;
        else if (levelStr == "error") level = LogLevel::ERR;

        EventManager& eventManager = EventManager::getInstance();

        if (parser.hasFlag("--console")) {
            auto consoleLogger = std::make_shared<ConsoleLogger>(level);
            eventManager.subscribe(consoleLogger);
            std::cout << "Console logging enabled (level: " << levelStr << ")" << std::endl;
        }

        if (parser.hasOption("--log")) {
            std::string logFile = parser.getOption("--log");

            auto fileLogger = std::make_shared<FileLogger>(logFile, level);
            if (fileLogger->isOpen()) {
                eventManager.subscribe(fileLogger);
                std::cout << "File logging enabled: " << logFile << std::endl;
            } else {
                std::cerr << "Failed to open log file: " << logFile << std::endl;

                if (!parser.hasFlag("--console")) {
                    auto fallbackLogger = std::make_shared<ConsoleLogger>(level);
                    eventManager.subscribe(fallbackLogger);
                    std::cout << "Falling back to console logging" << std::endl;
                }
            }
        }

        if (!parser.hasFlag("--console") && !parser.hasOption("--log")) {
            auto consoleLogger = std::make_shared<ConsoleLogger>(level);
            eventManager.subscribe(consoleLogger);
        }
    }

    std::cout << "Loading key configuration..." << std::endl;

    KeyConfig& config = KeyConfig::getInstance();
    bool loaded = config.loadConfig("keyconfig.cfg");

    if (!loaded) {
        std::cout << "Configuration file not found. Creating default..." << std::endl;

        config.resetToDefault();

        config.saveConfig("keyconfig.cfg");

        config.loadConfig("keyconfig.cfg");
    }

    std::cout << "=== LOADED KEY BINDINGS ===" << std::endl;
    std::vector<CommandType> allCmds = KeyConfig::getAllCommands();
    for (auto cmd : allCmds) {
        int key = config.getKey(cmd);
        if (key != -1) {
            std::string cmdName = KeyConfig::commandToString(cmd);
            std::string keyName = config.keyCodeToString(key);
            std::cout << cmdName << " -> " << keyName << " (code: " << key << ")" << std::endl;
        }
    }


    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/images/images/user1.jpg"));

    auto model = std::make_unique<GameModel>();

    auto view = std::make_unique<GameView>(model.get());

    auto visualizer = std::make_unique<Visualization<GameView>>(view.get(), model.get());

    auto controller = std::make_unique<GameController<InputHandler, GameView>>(*model, visualizer.get());

    view->set_handler(controller->getEventHandler());

    QMainWindow window;
    auto* raw_view = view.release();
    window.setCentralWidget(raw_view);
    window.setWindowTitle("Game");
    window.resize(650, 800);
    window.show();

    return app.exec();
}
