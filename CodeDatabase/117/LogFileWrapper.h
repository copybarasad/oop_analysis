#ifndef LOGFILEWRAPPER_H
#define LOGFILEWRAPPER_H

#include <fstream>
#include <string>

class LogFileWrapper {
private:
    std::fstream fileStream;
    std::string fileName;

public:
    LogFileWrapper(const std::string& filename);
    ~LogFileWrapper();

    LogFileWrapper(const LogFileWrapper&) = delete;
    LogFileWrapper& operator=(const LogFileWrapper&) = delete;
    LogFileWrapper(LogFileWrapper&&) = default;
    LogFileWrapper& operator=(LogFileWrapper&&) = default;
    void write(std::string message);
};

#endif