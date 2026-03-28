#pragma once
#include <string>

class InterfaceGameLogger {
public:
    virtual ~InterfaceGameLogger() = default;
    virtual void log(const std::string& message) = 0;
};