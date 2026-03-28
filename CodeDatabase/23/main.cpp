#include "EventLogger.h"
#include "EventSystem.h"
#include "GameExceptions.h"
#include "GameManager.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

enum class LogMode { NONE, FILE, CONSOLE, BOTH };

LogMode parseLogMode(int argc, char *argv[]) {
  if (argc < 2) {
    return LogMode::NONE;
  }

  std::string arg = argv[1];
  if (arg == "--log-file") {
    return LogMode::FILE;
  } else if (arg == "--log-console") {
    return LogMode::CONSOLE;
  } else if (arg == "--log-both") {
    return LogMode::BOTH;
  }

  return LogMode::NONE;
}

int main(int argc, char *argv[]) {
  std::cout << "Welcome to the Turn-Based Game!" << std::endl;

  LogMode logMode = parseLogMode(argc, argv);

  std::unique_ptr<EventLogger> logger;

  switch (logMode) {
  case LogMode::FILE: {
    std::cout << "Logging enabled: Writing to game_log.txt" << std::endl;
    logger = std::make_unique<FileLogger>("game_log.txt");
    break;
  }
  case LogMode::CONSOLE: {
    std::cout << "Logging enabled: Console output" << std::endl;
    logger = std::make_unique<ConsoleLogger>();
    break;
  }
  case LogMode::BOTH: {
    std::cout << "Logging enabled: File and Console output" << std::endl;
    auto composite = std::make_unique<CompositeLogger>();
    composite->addLogger(std::make_unique<FileLogger>("game_log.txt"));
    composite->addLogger(std::make_unique<ConsoleLogger>());
    logger = std::move(composite);
    break;
  }
  case LogMode::NONE:
  default:

    break;
  }

  if (logger) {
    EventSystem::initialize(std::move(logger));
  }

  try {
    GameManager manager;
    manager.startGame();
  } catch (const GameException &e) {
    std::cerr << "Game Error: " << e.what() << std::endl;
    EventSystem::shutdown();
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    EventSystem::shutdown();
    return 1;
  } catch (...) {
    std::cerr << "Unknown error occurred" << std::endl;
    EventSystem::shutdown();
    return 1;
  }

  EventSystem::shutdown();

  return 0;
}