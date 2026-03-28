#pragma once
#include <string>

class LogSink {
public:
    virtual ~LogSink() = default;
    virtual void Write(const std::string& line) = 0;
};
