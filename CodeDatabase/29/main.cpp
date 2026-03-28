#include "game_controller.hpp"
#include "input.hpp"
#include "event_bus.hpp"
#include "logger.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
	try {
		bool log_to_console = true;
		bool no_logs = false;
		std::string log_file;
		bool flags_specified = false;
		auto make_default_log_path = []() {
			std::time_t t = std::time(nullptr);
			std::tm tm{};
#ifdef _WIN32
			localtime_s(&tm, &t);
#else
			localtime_r(&t, &tm);
#endif
			std::ostringstream oss;
			oss << "game_log_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".txt";
			return oss.str();
		};
		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if (arg == "--nolog") {
				flags_specified = true;
				no_logs = true;
				log_to_console = false;
				log_file.clear();
			} else if (arg == "--logconsole") {
				flags_specified = true;
				log_to_console = true;
			} else if (arg.rfind("--logfile", 0) == 0) {
				flags_specified = true;
				log_file = arg.substr(std::string("--logfile").size());
			}
		}

		if (!flags_specified) {
			std::cout << "Logging destination:\n"
			          << "1) Console (default)\n"
			          << "2) File\n"
			          << "3) None\n"
			          << "4) Console + File\n"
			          << "Select option: ";
			std::string choice;
			std::getline(std::cin, choice);
			if (choice == "2") {
				log_to_console = false;
				no_logs = false;
				std::cout << "Enter log file path: ";
				std::getline(std::cin, log_file);
				if (log_file.empty()) {
					log_file = make_default_log_path();
					std::cout << "No path entered, using default log file: " << log_file << "\n";
					log_to_console = false;
				}
			} else if (choice == "3") {
				no_logs = true;
				log_to_console = false;
				log_file.clear();
			} else if (choice == "4") {
				no_logs = false;
				log_to_console = true;
				std::cout << "Enter log file path: ";
				std::getline(std::cin, log_file);
				if (log_file.empty()) {
					log_file = make_default_log_path();
					std::cout << "No path entered, using default log file: " << log_file << "\n";
				}
			} else {
				no_logs = false;
				log_to_console = true;
				log_file.clear();
			}
		}

		EventBus bus;
		if (!no_logs && log_to_console) {
			bus.AddSink(std::make_unique<StdoutEventObserver>());
		}
		if (!no_logs) {
			if (log_file.empty()) {
				log_file = make_default_log_path();
				std::cout << "Using default log file: " << log_file << "\n";
			}
			auto sink = std::make_unique<FileEventObserver>(log_file);
			if (sink->IsOpen()) {
				bus.AddSink(std::move(sink));
				if (!flags_specified) {
					std::cout << "Logging to file: " << log_file << "\n";
				}
			} else {
				std::cerr << "Cannot open log file '" << log_file << "'\n";
			}
		}

		Input io(std::cin, std::cout);
		GameController controller(io, "savegame.txt", &bus);
		controller.Run();
	} catch (const std::exception& ex) {
		std::cerr << "Fatal error: " << ex.what() << "\n";
		return 1;
	}
	return 0;
}
