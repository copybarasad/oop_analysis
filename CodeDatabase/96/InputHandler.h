#pragma once
#include <string>
#include <sstream>
#include <iostream>

#include <termios.h>
#include <unistd.h>
#include <cstdio>

#include "Command.h"


class ConsoleInput {
private:
    struct termios oldt, newt;
    char getChar() {
        char ch = 0;
        read(STDIN_FILENO, &ch, 1);
        return ch;
    }
public:
    ConsoleInput() {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        

        newt.c_lflag &= ~ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
    
    ~ConsoleInput() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
    
    Command getCommand(int& a1, int& a2);
};


