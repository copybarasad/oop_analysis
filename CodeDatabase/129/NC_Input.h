#pragma once
#include "Input.h"
#include <ncurses.h>

class NC_Input : public Input{
public:
    char get_input();
};
