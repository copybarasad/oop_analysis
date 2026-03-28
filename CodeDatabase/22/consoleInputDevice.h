#pragma once
#include "controllers/command.h"
#include "controllers/IInputHandler.h"
#include "console/keyMapper.h"
#include <unordered_map>
#include <iostream>
#include <string>

class Field;
class IGameView;

struct ConsoleInputDevice 
{
    std::unordered_map<char, CommandType> keymap;

    ConsoleInputDevice(const std::string &configFile = "config.json")
        : keymap(KeyMapper::loadConfig(configFile))
    {
    }

    Command getCommand();

    Command selectIndex(int max);

    Command readString();

    Command selectPosition(Field &field, int radius, IGameView &render);

    Command readOptions();
};
