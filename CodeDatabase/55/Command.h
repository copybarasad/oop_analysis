#pragma once
#include "Enums.h"
#include <string>


struct Command {
    CommandID id = CommandID::None;
    int dx = 0, dy = 0;
    int x = 0, y = 0;
    int spellIndex = -1;
    int levelIndex = -1;
    int upgradeIndex = -1;
    std::string path = "save.dat";
};