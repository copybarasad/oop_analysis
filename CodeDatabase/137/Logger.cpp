#include "Logger.h"
#include "GameException.h"
#include <chrono>
#include <iomanip>
#include <sstream>

FileLogWriter::FileLogWriter(const std::string& filename) {
    file_.open(filename, std::ios::app);
    if (!file_.is_open()) {
        throw FileOpenException(filename);
    }
}

FileLogWriter::~FileLogWriter() {
    if (file_.is_open()) {
        file_.close();
    }
}

void FileLogWriter::Write(const std::string& message) {
    if (file_.is_open()) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        file_ << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") 
              << " " << message << std::endl;
    }
}

void FileLogWriter::Flush() {
    if (file_.is_open()) {
        file_.flush();
    }
}

void ConsoleLogWriter::Write(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleLogWriter::Flush() {
    std::cout.flush();
}

Logger::Logger() : enabled_(true) {}

Logger::~Logger() = default;

void Logger::AddWriter(std::unique_ptr<ILogWriter> writer) {
    writers_.push_back(std::move(writer));
}

void Logger::Log(const GameEvent& event) {
    if (!enabled_) return;
    
    std::string message = event.ToString();
    for (auto& writer : writers_) {
        writer->Write(message);
    }
}

void Logger::Log(const std::string& message) {
    if (!enabled_) return;
    
    for (auto& writer : writers_) {
        writer->Write(message);
    }
}

void Logger::SetEnabled(bool enabled) {
    enabled_ = enabled;
}

bool Logger::IsEnabled() const {
    return enabled_;
}

LoggerObserver::LoggerObserver(Logger& logger) : logger_(logger) {}

void LoggerObserver::OnEvent(const GameEvent& event) {
    logger_.Log(event);
}
