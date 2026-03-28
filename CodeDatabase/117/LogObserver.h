#ifndef LOGOBSERVER_H
#define LOGOBSERVER_H

#include "IObserver.h"
#include "LogFileWrapper.h"
#include <memory>

enum class LogType {InFile, Console};

class LogObserver : public IObserver {
private:
    LogType type;
    std::unique_ptr<LogFileWrapper> logFile;

public:
    LogObserver(LogType type, const std::string& filename);
    void seeEvent(const Event& event) override;
};

#endif