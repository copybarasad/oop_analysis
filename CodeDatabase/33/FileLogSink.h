#pragma once
#include <fstream>
#include <string>
#include "LogSink.h"

class FileLogSink : public LogSink {
public:
    explicit FileLogSink(const std::string& path);
    ~FileLogSink() override;
    void Write(const std::string& line) override;

private:
    std::ofstream out_;
};
