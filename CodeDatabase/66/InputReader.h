#ifndef INPUTREADER_H
#define INPUTREADER_H

#include "Command.h"
#include "KeyBindingConfig.h"
#include <memory>
#include <string>

class SpellHand;
class Game;

template<typename TInputHandler>
class InputReader {
private:
    TInputHandler inputHandler;
    KeyBindingConfig& keyBindings;
    
public:
    InputReader() : keyBindings(KeyBindingConfig::getInstance()) {}
    
    explicit InputReader(KeyBindingConfig& config) : keyBindings(config) {}
    
    std::unique_ptr<Command> readCommand(const SpellHand* spellHand = nullptr) {
        return inputHandler.readAndConvert(keyBindings, spellHand);
    }
    
    bool processInput(Game& game, const SpellHand* spellHand = nullptr) {
        std::unique_ptr<Command> cmd = readCommand(spellHand);
        if (!cmd) {
            return false;
        }
        return cmd->execute(game);
    }
    
    TInputHandler& getHandler() { return inputHandler; }
    const TInputHandler& getHandler() const { return inputHandler; }
};

class ConsoleInputHandler {
public:
    std::unique_ptr<Command> readAndConvert(KeyBindingConfig& keyBindings, 
                                            const SpellHand* spellHand);
    
private:
    Direction parseDirectionInput(KeyBindingConfig& keyBindings);
    Position parsePositionInput();
    int parseSpellIndex(const SpellHand* spellHand);
    std::string readLine();
    char readChar();
};

#endif
