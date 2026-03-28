#ifndef COMMAND_INPUT_H
#define COMMAND_INPUT_H

#include "DefaultInputStrategy.h"  
#include "PlayerCommand.h"
#include <string>
#include <map>
#include <vector>

template<typename T = DefaultInputStrategy>
class CommandInput {
private:
    T inputStrategy;
    
public:
    CommandInput() {
        inputStrategy.loadKeyBindingsFromFile("controls.cfg");
    }
    
    PlayerCommand getCommand() {
        return inputStrategy.getCommand();
    }
    
    void showControls() {
        inputStrategy.showHelp();
    }
};

#endif 