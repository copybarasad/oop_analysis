#include "EventSystem.h"

EventSystem *EventSystem::instance = nullptr;

EventSystem &EventSystem::getInstance() {
  if (!instance) {
    instance = new EventSystem();
  }
  return *instance;
}

void EventSystem::initialize(std::unique_ptr<EventLogger> newLogger) {
  getInstance().logger = std::move(newLogger);
}

void EventSystem::shutdown() {
  if (instance) {
    if (instance->logger) {
      instance->logger->flush();
    }
    delete instance;
    instance = nullptr;
  }
}

void EventSystem::notifyEvent(const GameEvent &event) {
  if (logger) {
    logger->logEvent(event);
  }
}
