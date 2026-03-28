#include "FileLogSink.h"

FileLogSink::FileLogSink(const std::string& path)
    : out_(path, std::ios::out | std::ios::app) {}

FileLogSink::~FileLogSink() {
    if (out_.is_open()) {
        out_.flush();
        out_.close();
    }
}

void FileLogSink::Write(const std::string& line) {
    if (out_.is_open()) {
        out_ << line << std::endl;
    }
}
