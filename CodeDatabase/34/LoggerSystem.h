#ifndef GAME_LOGGERSYSTEM_H
#define GAME_LOGGERSYSTEM_H

#include "IObserver.h"
#include "ILogger.h"
#include <memory>
#include <string>

namespace Game {

class LoggerSystem : public IObserver {
public:
    LoggerSystem(std::unique_ptr<ILogger> logger);
    void onNotify(const Event& event) override;

private:
    std::string formatEvent(const Event& event) const;
    std::unique_ptr<ILogger> loggerStrategy;
};

}

#endif