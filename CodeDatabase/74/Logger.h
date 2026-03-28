#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <mutex>

class Logger {
public:
    static void init(const std::string& filename = "game_log.txt") {
        logFile.open(filename, std::ios::out | std::ios::trunc);
        if (!logFile.is_open()) {
            std::cerr << "Cannot open log file: " << filename << std::endl;
        }
    }

    static void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mtx);
        if (logFile.is_open()) {
            logFile << message;
            logFile.flush();
        }
    }

    class Stream {
        std::ostringstream buffer;
    public:
        ~Stream() {
            std::lock_guard<std::mutex> lock(Logger::mtx);
            if (Logger::logFile.is_open()) {
                Logger::logFile << buffer.str();
                Logger::logFile.flush();
            }
        }

        template <typename T>
        Stream& operator<<(const T& value) {
            buffer << value;
            return *this;
        }
    };

    static Stream log() { return Stream(); }

    static void close() {
        std::lock_guard<std::mutex> lock(mtx);
        if (logFile.is_open()) logFile.close();
    }

private:
    static inline std::ofstream logFile;
    static inline std::mutex mtx;
};
