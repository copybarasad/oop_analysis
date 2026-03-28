#include "event_bus.hpp"

void EventBus::AddSink(std::unique_ptr<EventObserver> sink) {
	if (sink) sinks_.push_back(std::move(sink));
}

void EventBus::Publish(const Event& ev) {
	for (auto& s : sinks_) {
		s->Log(ev);
	}
}
