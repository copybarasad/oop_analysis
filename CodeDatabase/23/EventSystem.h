#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "EventLogger.h"
#include "GameEvent.h"
#include <memory>


class EventSystem {
private:
  static EventSystem *instance;
  std::unique_ptr<EventLogger> logger;

  EventSystem() = default;

public:
  
  static EventSystem &getInstance();
  static void initialize(std::unique_ptr<EventLogger> logger);
  static void shutdown();

  
  void notifyEvent(const GameEvent &event);

  
  bool isLoggingEnabled() const { return logger != nullptr; }

  
  EventSystem(const EventSystem &) = delete;
  EventSystem &operator=(const EventSystem &) = delete;
};

#endif 
