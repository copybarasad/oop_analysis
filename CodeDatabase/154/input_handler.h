#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "command.h"
#include "key_bindings_config.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>

class InputHandlerInterface {
public:
    virtual ~InputHandlerInterface() = default;
    virtual Command getCommand() = 0;
};

class ConsoleInputHandler : public InputHandlerInterface {
private:
    std::map<char, CommandType> keyBindings_;

    void loadDefaultBindings() {
        keyBindings_['w'] = CommandType::MoveUp;
        keyBindings_['a'] = CommandType::MoveLeft;
        keyBindings_['s'] = CommandType::MoveDown;
        keyBindings_['d'] = CommandType::MoveRight;
        keyBindings_['e'] = CommandType::Attack;
        keyBindings_['c'] = CommandType::CastSpell;
        keyBindings_['q'] = CommandType::ShowInfo;
        keyBindings_['p'] = CommandType::SaveGame;
        keyBindings_['l'] = CommandType::LoadGame;
        keyBindings_['x'] = CommandType::Quit;
    }

    bool validateBindings(const std::map<char, CommandType>& bindings) {
        std::map<CommandType, char> reverseMap;

        for (const auto& pair : bindings) {
            char key = pair.first;
            CommandType cmd = pair.second;

            if (reverseMap.count(cmd) && reverseMap[cmd] != key) {
                return false;  
            }
            reverseMap[cmd] = key;
        }

        std::vector<CommandType> requiredCommands = {
            CommandType::MoveUp, CommandType::MoveDown, CommandType::MoveLeft, CommandType::MoveRight,
            CommandType::Attack, CommandType::SaveGame, CommandType::Quit
        };

        for (CommandType cmd : requiredCommands) {
            if (reverseMap.find(cmd) == reverseMap.end()) {
                return false;  
            }
        }

        return true;
    }

public:
    ConsoleInputHandler() {
        loadDefaultBindings();
    }

    bool loadFromFile(const std::string& configFile) {
        std::map<char, CommandType> newBindings;
        if (KeyBindingsConfig::loadConfigFile(configFile, newBindings)) {
            keyBindings_ = newBindings;
            return true;
        }
        loadDefaultBindings();
        return false;
    }

    Command getCommand() override {
        char ch;
        if (std::cin >> ch) {
            auto it = keyBindings_.find(ch);
            if (it != keyBindings_.end()) {
                return Command(it->second);
            }
        }
        return Command(CommandType::None);
    }

    bool setKeyBindings(const std::map<char, CommandType>& newBindings) {
        if (validateBindings(newBindings)) {
            keyBindings_ = newBindings;
            return true;
        }
        
        return false;
    }

    const std::map<char, CommandType>& getKeyBindings() const {
        return keyBindings_;
    }
};

#endif
