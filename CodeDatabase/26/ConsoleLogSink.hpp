#pragma once

#include <ostream>

#include "LogSink.hpp"

class ConsoleLogSink : public LogSink {
public:
	explicit ConsoleLogSink(std::ostream& stream);

	void write(const std::string& message) override;

private:
	std::ostream& stream_;
};
