#pragma once
#include "ILogger.h"
#include <fstream>

class FileLogger : public ILogger
{
    std::ofstream file;

public:
    FileLogger(const std::string &filename);

    void log(const Event &event) override;
    
};