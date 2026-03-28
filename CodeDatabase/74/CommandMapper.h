#pragma once
#include <map>
#include <fstream>
#include <string>
#include "Command.h"

class CommandMapper {
private:
    std::map<char, CommandType> keyMap;

public:
    CommandMapper() { loadDefaultControls(); }

    void loadDefaultControls() {
        keyMap.clear();
        keyMap['w'] = CommandType::MoveUp;
        keyMap['s'] = CommandType::MoveDown;
        keyMap['a'] = CommandType::MoveLeft;
        keyMap['d'] = CommandType::MoveRight;
        keyMap['e'] = CommandType::Action;
        keyMap['y'] = CommandType::Save;
        keyMap['q'] = CommandType::SwitchMode;
        keyMap['r'] = CommandType::OpenHand;
        for (int i = 0; i <= 9; ++i) {
            keyMap['0' + i] = CommandType::SelectSpell;
        }
    }

    Command mapKey(char input) {
        input = static_cast<char>(std::tolower(static_cast<unsigned char>(input)));
        if (keyMap.count(input)) {
            if (input >= '0' && input <= '9') {
                return Command(keyMap[input], input - '0');
            }
            return Command(keyMap[input]);
        }
        return Command(CommandType::None);
    }
};