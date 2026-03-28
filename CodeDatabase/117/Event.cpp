#include "Event.h"

Event::Event() : eventTime(std::time(nullptr)) {}

time_t Event::getTime() const {
    return this->eventTime;
}