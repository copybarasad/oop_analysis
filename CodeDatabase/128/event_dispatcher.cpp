#include "event_dispatcher.hpp"

EventDispatcher& EventDispatcher::instance() {
    static EventDispatcher dispatcher;
    return dispatcher;
}
