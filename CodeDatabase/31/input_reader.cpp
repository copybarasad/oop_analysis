#include "input_reader.hpp"
#include "command.hpp"
#include "field/point.hpp"
#include <cstdio>


Command InputReader::read_input() {
    int ch = fgetc(stdin);
    
    if (ch == '\033') {
        fgetc(stdin); 
        switch(fgetc(stdin)) {
            case 'A': return Command::Up;
            case 'B': return Command::Down;
            case 'C': return Command::Right;
            case 'D': return Command::Left;
            default: return Command::Exit;
        }
    }
    
    switch (ch) {
        case ' ': return Command::Attack;
        case 'q': return Command::Exit;
        case '\n': return Command::SwitchAttackType;
        case '1': return Command::ShowHand;
        case '2': return Command::BuySpell;
        case '3': return Command::UseSpell;
        case 's': return Command::Save;
        case 'l': return Command::Load;
        case 'h': return Command::Help;
        default: return Command::Exit;
    }
}

bool InputReader::move_cursor(Command command, Point& point, int xmin, int xmax, int ymin, int ymax) {
    switch (command) {
        case Command::Up: 
            if (point.y > ymin) { point.y--; return true; }
            break;
        case Command::Down: 
            if (point.y < ymax) { point.y++; return true; }
            break;
        case Command::Left: 
            if (point.x > xmin) { point.x--; return true; }
            break;
        case Command::Right: 
            if (point.x < xmax) { point.x++; return true; }
            break;
        default: 
            return false;
    }
    return false;
}

Command get_confirm_command() { 
    return Command::Attack;
}
