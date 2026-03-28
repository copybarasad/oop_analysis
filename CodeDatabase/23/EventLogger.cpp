#include "EventLogger.h"
#include <iostream>

FileLogger::FileLogger(const std::string &filename) : filename(filename) {
  logFile.open(filename, std::ios::app);
  if (logFile.is_open()) {
    logFile << "\n=== New Game Session Started ===" << std::endl;
  }
}

FileLogger::~FileLogger() {
  if (logFile.is_open()) {
    logFile << "=== Game Session Ended ===" << std::endl;
    logFile.close();
  }
}

void FileLogger::logEvent(const GameEvent &event) {
  if (logFile.is_open()) {
    logFile << "[" << event.getFormattedTime() << "] "
            << "[" << event.getEventType() << "] " << event.getDescription()
            << std::endl;
  }
}

void FileLogger::flush() {
  if (logFile.is_open()) {
    logFile.flush();
  }
}

void ConsoleLogger::logEvent(const GameEvent &event) {
  std::cerr << "[LOG] [" << event.getEventType() << "] "
            << event.getDescription() << std::endl;
}

void ConsoleLogger::flush() { std::cerr.flush(); }

void CompositeLogger::addLogger(std::unique_ptr<EventLogger> logger) {
  loggers.push_back(std::move(logger));
}

void CompositeLogger::logEvent(const GameEvent &event) {
  for (auto &logger : loggers) {
    logger->logEvent(event);
  }
}

void CompositeLogger::flush() {
  for (auto &logger : loggers) {
    logger->flush();
  }
}
