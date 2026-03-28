#pragma once

#include <fstream>
#include <string>

#include "LogSink.hpp"

class FileLogSink : public LogSink {
public:
	explicit FileLogSink(const std::string& path);

	void write(const std::string& message) override;
	bool isOpen() const noexcept;

private:
	std::ofstream file_;
};
