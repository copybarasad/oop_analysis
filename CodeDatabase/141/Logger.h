#pragma once

#include "EventBus.h"
#include <memory>
#include <string>
#include <fstream>
#include <iostream>

class ILogSink {
public:
    virtual ~ILogSink() = default;

    virtual void write(const std::string &line) = 0;
};

class ConsoleSink : public ILogSink {
public:
    void write(const std::string &line) override {
        std::cout << line << std::endl;
    }
};

class FileSink : public ILogSink {
public:
    explicit FileSink(const std::string &path) : out(path, std::ios::app) {
    }

    void write(const std::string &line) override {
        if (out.is_open()) {
            out << line << std::endl;
            out.flush();
        }
    }

private:
    std::ofstream out;
};

class Logger {
public:
    explicit Logger(std::shared_ptr<ILogSink> sink) : sink(std::move(sink)) {
    }

    void attach(EventBus &bus) {
        // Подписка на события игры
        bus.subscribe([this](const GameEvent &event) { handle(event); });
    }

private:
    std::shared_ptr<ILogSink> sink;

    void handle(const GameEvent &event);
};
