#ifndef GAME_FILELOGGER_H
#define GAME_FILELOGGER_H

#include "ILogger.h"
#include <string>
#include <fstream>
#include <memory>

namespace Game {

class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename);
    void log(const std::string& message) override;
private:
    std::ofstream fileStream;
};

}

#endif
