#include "ConsoleLogSink.hpp"

ConsoleLogSink::ConsoleLogSink(std::ostream& stream)
	: stream_(stream) {}

void ConsoleLogSink::write(const std::string& message) {
	stream_ << message << std::endl;
}
