#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ConsoleCommandSource.hpp"
#include "ConsoleLogSink.hpp"
#include "ControlScheme.hpp"
#include "EventBus.hpp"
#include "FileLogSink.hpp"
#include "Game.hpp"
#include "GameController.hpp"
#include "GameLogger.hpp"
#include "GameVisualizer.hpp"
#include "Renderer.hpp"

bool g_verbose = false;

namespace {
struct LaunchOptions {
	std::string logMode = "console";
	std::string logPath = "game.log";
	std::string controlsPath = "controls.cfg";
};

LaunchOptions parseOptions(int argc, char** argv) {
	LaunchOptions opts;
	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg.rfind("--log=", 0) == 0) {
			opts.logMode = arg.substr(6);
		} else if (arg.rfind("--controls=", 0) == 0) {
			opts.controlsPath = arg.substr(11);
		}
	}
	return opts;
}
} // namespace

int main(int argc, char** argv) {
	const char* env = std::getenv("GAME_VERBOSE");
	if (env && std::string(env) == "1") {
		g_verbose = true;
	}

	const LaunchOptions options = parseOptions(argc, argv);

	try {
		EventBus bus;

		std::shared_ptr<LogSink> sink;
		if (options.logMode.rfind("file", 0) == 0) {
			std::string path = options.logPath;
			auto colon = options.logMode.find(':');
			if (colon != std::string::npos && colon + 1 < options.logMode.size()) {
				path = options.logMode.substr(colon + 1);
			}
			auto fileSink = std::make_shared<FileLogSink>(path);
			if (!fileSink->isOpen()) {
				std::cerr << "Failed to open log file \"" << path << "\", falling back to console.\n";
				sink = std::make_shared<ConsoleLogSink>(std::cout);
			} else {
				sink = fileSink;
			}
		} else {
			sink = std::make_shared<ConsoleLogSink>(std::cout);
		}

		GameLogger logger(bus, sink);

		Renderer::initConsoleOnce();
		Renderer renderer(Renderer::shouldUseAsciiFallback() ? Renderer::Theme::Ascii : Renderer::Theme::Fancy);
		GameVisualizer<Renderer> visualizer(bus, std::move(renderer), std::cout);

		ControlScheme scheme;
		if (!scheme.loadFromFile(options.controlsPath)) {
			std::cout << "Using default controls.\n";
		}

		ConsoleCommandSource input(scheme, std::cin, std::cout);
		Game game(bus, scheme);
		GameController<ConsoleCommandSource> controller(game, input, bus);
		controller.run();
	} catch (const std::exception& ex) {
		std::cerr << "Fatal error: " << ex.what() << "\n";
		return 1;
	}

	return 0;
}
