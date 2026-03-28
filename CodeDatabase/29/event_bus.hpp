#ifndef EVENT_BUS_HPP
#define EVENT_BUS_HPP

#include <vector>
#include <memory>
#include "event.hpp"

class EventObserver {
public:
	virtual ~EventObserver() = default;
	virtual void Log(const Event& ev) = 0;
};

class EventBus {
public:
	void AddSink(std::unique_ptr<EventObserver> sink);
	void Publish(const Event& ev);
private:
	std::vector<std::unique_ptr<EventObserver>> sinks_;
};

#endif
