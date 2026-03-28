#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include "GameEvent.h"
#include <fstream>
#include <memory>
#include <string>
#include <vector>


class EventLogger {
public:
  virtual ~EventLogger() = default;
  virtual void logEvent(const GameEvent &event) = 0;
  virtual void flush() = 0;
};


class FileLogger : public EventLogger {
private:
  std::ofstream logFile;
  std::string filename;

public:
  FileLogger(const std::string &filename);
  ~FileLogger();
  void logEvent(const GameEvent &event) override;
  void flush() override;
};


class ConsoleLogger : public EventLogger {
public:
  void logEvent(const GameEvent &event) override;
  void flush() override;
};


class CompositeLogger : public EventLogger {
private:
  std::vector<std::unique_ptr<EventLogger>> loggers;

public:
  void addLogger(std::unique_ptr<EventLogger> logger);
  void logEvent(const GameEvent &event) override;
  void flush() override;
};

#endif 
