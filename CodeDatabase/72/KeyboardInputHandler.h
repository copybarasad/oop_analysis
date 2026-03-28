#ifndef GAME_TERM_KEYBOARDINPUTHANDLER_H
#define GAME_TERM_KEYBOARDINPUTHANDLER_H

#include <map>
#include <memory>
#include <string>
#include "../commands/CommandType.h"
#include "../Direction.h"


class Command;

class KeyboardInputHandler {
    std::map<char, CommandType> keyBindings;
    std::string configFile;

    void loadConfig(const std::string& filename);
    void setDefaultBindings();
    bool validateConfig() const;
    std::unique_ptr<Command> createCommand(CommandType type) const;
    void clearLine() const;

public:
    explicit KeyboardInputHandler(const std::string& configFile = "/Users/mac/CLionProjects/rpg-game/configuration/configuration.txt");

    std::unique_ptr<Command> getNextCommand();

    Direction getShootDirection() const;
    std::unique_ptr<Command> getSaveMenuCommand();

};

#endif // GAME_TERM_KEYBOARDINPUTHANDLER_H