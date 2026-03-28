#ifndef INPUT_READER_H
#define INPUT_READER_H

#include <memory>
#include <map>
#include <string>
#include "Commands.h"

enum class ActionType {
    kMoveUp, kMoveDown, kMoveLeft, kMoveRight,
    kAction,
    kSwitchStance,
    kCastMode,
    kBuy,
    kSave,
    kLoad,
    kQuit,
    kCancel,
    kNone
};

class InputReader {
public:
    InputReader();
    std::unique_ptr<Command> readCommand();
    void loadConfig(const std::string& filename);
    
    std::string getControlsDescription() const;

private:
    std::map<char, ActionType> key_map_;
    
    void parseConfigLine(const std::string& line);
    std::unique_ptr<Command> createCommand(ActionType action);
    std::string actionToString(ActionType action) const;
};

#endif