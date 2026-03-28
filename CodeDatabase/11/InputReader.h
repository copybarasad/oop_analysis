#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <string>
#include <map>
#include <memory>
#include "Commands.h"

enum class ActionType {
    NONE, 
    MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
    SWITCH_MODE, CAST_SPELL, BUY_SPELL, 
    SAVE, LOAD, EXIT
};

class InputReader {
public:
    InputReader();
    void loadConfig(const std::string& filename);
    std::unique_ptr<ICommand> readCommand();

private:
    std::map<char, ActionType> keyMap;
    
    void setDefaultConfig();
    std::unique_ptr<ICommand> createCommand(ActionType action);
};

#endif