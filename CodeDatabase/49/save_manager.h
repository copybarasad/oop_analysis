#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <fstream>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include "save_exception.h"
#include "load_exception.h"
#include "file_ioe_exception.h"
#include "../game/game.h"

class Game;

class SaveManager
{
public:
    static void save_game(const Game& game, const std::string& path);

    static void load_game(Game& game, const std::string& path);
};

#endif