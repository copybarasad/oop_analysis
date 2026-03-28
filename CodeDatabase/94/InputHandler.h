#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Command.h"
#include "CommandParser.h"  
#include "Game.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdio>

// Консольная реализация обработчика ввода 
class ConsoleInputHandler {
private:
    const Game& game;
    CommandParser parser;
    
    char getInput() {
        char input = '\0';
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        input = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return input;
    }
    
public:
    explicit ConsoleInputHandler(const Game& g) : game(g) {}
    
    Command getCommand() {
        char input = getInput();
        return parser.parse(game.getStatus(), input);
    }
};

#endif