#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "event.hpp"
#include "event_bus.hpp"
#include <fstream>
#include <iostream>

class StdoutEventObserver : public EventObserver {
public:
	void Log(const Event& ev) override;
};

class FileEventObserver : public EventObserver {
public:
	explicit FileEventObserver(const std::string& path);
	void Log(const Event& ev) override;
	bool IsOpen() const noexcept { return out_.is_open(); }
private:
	std::ofstream out_;
};

#endif
