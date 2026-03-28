#include <iostream>
#include <memory>
#include <string>
#include "game.h"
#include "control_config.h"
#include "game_logging.h"

int main(int argc, char* argv[]) {
	std::string controlFile;
	std::string logMode = "console";
	std::string logFilePath = "game.log";

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "--controls" && i + 1 < argc) {
			controlFile = argv[++i];
		} else if (arg == "--log" && i + 1 < argc) {
			logMode = argv[++i];
		} else if (arg == "--log-file" && i + 1 < argc) {
			logFilePath = argv[++i];
		}
	}

	ControlConfig controls = controlFile.empty()
		? ControlConfig::createDefault()
		: ControlConfig::loadFromFile(controlFile);

	std::shared_ptr<GameLogSink> sink;
	if (logMode == "file") {
		sink = std::make_shared<FileLogSink>(logFilePath);
	} else if (logMode == "silent") {
		sink = nullptr;
	} else {
		sink = std::make_shared<ConsoleLogSink>();
	}

    int menuChoice = 0;
    std::cout << "1. Start New Game\n";
    std::cout << "2. Load Saved Game\n";
    std::cout << "Select option: ";
    std::cin >> menuChoice;

    int width = 15;
    int height = 15;
    if (menuChoice == 1) {
        std::cout << "Enter field width (10-25): ";
        std::cin >> width;
        if (width < 10) width = 10;
        if (width > 25) width = 25;
        std::cout << "Enter field height (10-25): ";
        std::cin >> height;
        if (height < 10) height = 10;
        if (height > 25) height = 25;
    }

    Game game(width, height, controls, sink);

    if (menuChoice == 2) {
        game.loadCampaign();
    }

    game.runGameLoop();
    
    return 0;
}