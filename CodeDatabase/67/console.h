#ifndef CONSOLE_H
#define CONSOLE_H

#include "commands.h"
#include <iostream>
#include <memory> 
#include <limits>

class ConsoleInputReader {
public:
    std::unique_ptr<Command> readCommand(game& g) {
        char input;
        std::cout << "Enter command (WASD, C - cast spells, H - do nthng, V - save, L - load, Q - exit): ";
        std::cin >> input;

        switch (input) {

            case 'w': case 'W': return std::make_unique<MoveCommand>(0, -1);
            case 's': case 'S': return std::make_unique<MoveCommand>(0, 1);
            case 'a': case 'A': return std::make_unique<MoveCommand>(-1, 0);
            case 'd': case 'D': return std::make_unique<MoveCommand>(1, 0);
            
            case 'c': case 'C': return std::make_unique<CastSpellCommand>();
            case 'h': case 'H': return std::make_unique<SkipTurnCommand>();
            
            case 'v': case 'V': return std::make_unique<SaveCommand>();
            case 'l': case 'L': return std::make_unique<LoadCommand>();
            case 'q': case 'Q': return std::make_unique<ExitCommand>();

            default: 
                g.add_log("Unknown command: " + std::string(1, input));
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return nullptr;
        }
    }
};

#endif
