#ifndef FILELOGGER_H
#define FILELOGGER_H
#include "EventListener.h"
#include <fstream>


class FileLogger : public EventListener {
private:
    std::ofstream logFile_;
    LogLevel minLevel_;
    std::string filename_;

public:
    FileLogger(const std::string& filename, LogLevel minLevel = LogLevel::INFO);

    ~FileLogger();

    void onEvent(const std::shared_ptr<GameEvent>& event) override;

    LogLevel getMinLogLevel() const override;

    void setMinLevel(LogLevel level);

    bool isOpen() const;
    void close();
};

#endif // FILELOGGER_H
