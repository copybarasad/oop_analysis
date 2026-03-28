#include "LogFileWrapper.h"
#include <stdexcept>

LogFileWrapper::LogFileWrapper(const std::string& filename): fileName(filename) {
    fileStream.exceptions(std::ios::badbit);
    fileStream.open(fileName, std::ios::app);
    if (!fileStream.is_open()) {
        throw std::runtime_error("Can't open log file " + fileName);
    }
}

LogFileWrapper::~LogFileWrapper() {
    if (fileStream.is_open()) {
        try {
            fileStream.close();
        } catch (...) {}
    }
}

void LogFileWrapper::write(std::string message){
    try {
        fileStream << message << std::endl;
    } catch (const std::exception& e) {
        throw std::runtime_error("Error writing logs to file: " + std::string(e.what()));
    }
}