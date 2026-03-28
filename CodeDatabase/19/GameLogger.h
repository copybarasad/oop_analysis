#pragma once
#include <fstream>
#include <iostream>

#include "LoggingMode.h"
#include "event/IEventListener.h"


class GameLogger : public IEventListener {
public:
    GameLogger(LoggingMode dest, const std::string &filename = "log.txt") : mode_(dest) {
        if (mode_ == LoggingMode::File) {
            file_.open(filename, std::ios::out | std::ios::app);
            if (!file_.is_open()) {
                std::cerr << "Warning: Could not open log file, falling back to Console.\n";
                mode_ = LoggingMode::Console;
            }
        }
    }

    ~GameLogger() override {
        if (file_.is_open()) {
            file_.close();
        }
    }

    void handle_event(const GameEvent &event) override {
        std::string log_message = format_log(event);

        if (mode_ == LoggingMode::Console) {
            std::cout << log_message << std::endl;
        } else if (mode_ == LoggingMode::File) {
            file_ << log_message << std::endl;
        }
    }

private:
    LoggingMode mode_;
    std::ofstream file_;

    std::string format_log(const GameEvent &event) const {
        return "[" + event.type() + "] " + event.details();
    }
};
