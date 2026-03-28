#include "filelogger.h"

FileLogger::FileLogger(const std::string& filename, LogLevel minLevel)
    : minLevel_(minLevel), filename_(filename) {
    logFile_.open(filename, std::ios::app);
}

FileLogger::~FileLogger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void FileLogger::onEvent(const std::shared_ptr<GameEvent>& event) {
    if (logFile_.is_open()) {
        logFile_ << event->toFormattedString() << std::endl;
        logFile_.flush();
    }
}

LogLevel FileLogger::getMinLogLevel() const { return minLevel_; }

void FileLogger::setMinLevel(LogLevel level) { minLevel_ = level; }

bool FileLogger::isOpen() const { return logFile_.is_open(); }
void FileLogger::close() { if (logFile_.is_open()) logFile_.close(); }
