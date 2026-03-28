#pragma once
#include "LogSink.h"
#include <string>

class ConsoleLogSink : public LogSink {
public:
    void Write(const std::string& line) override;
};
