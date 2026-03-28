#ifndef ICOMMANDREADER_H
#define ICOMMANDREADER_H

#include "GameCommand.h"
#include <memory>

class ICommandReader {
public:
    virtual ~ICommandReader() = default;
    
    virtual GameCommand readCommand() = 0;
    virtual void displayHelp() = 0;
    virtual void loadConfiguration(const std::string& filename) = 0;
};

#endif
