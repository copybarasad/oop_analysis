#pragma once
#include "ILogger.h"
#include <fstream>
#include <string>

class FileLogger : public ILogger {
public:
    explicit FileLogger(const std::string& filename);
    ~FileLogger() override;

    void Log(const std::string& message) override;

private:
    std::ofstream out_;
};
