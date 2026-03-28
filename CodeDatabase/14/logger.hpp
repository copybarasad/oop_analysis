#ifndef LOGGER_H
#define LOGGER_H

#include "observer.hpp"
#include <iostream>
#include <string>

class GameLogger : public IObserver {
private:
    std::ostream& outStream;

public:
    explicit GameLogger(std::ostream& stream);
    void onNotify(EventType type, const std::string& message) override;
};

#endif