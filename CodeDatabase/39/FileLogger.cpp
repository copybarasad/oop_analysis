#include "FileLogger.h"
#include <iostream>

FileLogger::FileLogger(const std::string& filename)
    : out_(filename, std::ios::app)
{
    if (!out_) {
        std::cerr << "Cannot open log file: " << filename << "\n";
    }
}

FileLogger::~FileLogger() {
    if (out_) {
        out_.flush();
    }
}

void FileLogger::Log(const std::string& message) {
    if (out_) {
        out_ << message << '\n';
    }
}
