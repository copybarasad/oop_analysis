#include "InputReader.h"
#include <iostream>
#include <limits>
#include <cctype> 

GameCommand InputReader::readCommand() {
    char c;
    std::cin >> c;

    if (std::cin.fail()) {
        std::cin.clear(); 
    }

    GameCommand cmd;  

    char upperC = std::toupper(c);

    switch (upperC) {
        case 'W': cmd.type = CommandType::MoveUp; break;
        case 'S': cmd.type = CommandType::MoveDown; break;
        case 'A': cmd.type = CommandType::MoveLeft; break;
        case 'D': cmd.type = CommandType::MoveRight; break;
        case 'F': cmd.type = CommandType::AttackEnemy; break;
        case 'T': cmd.type = CommandType::AttackTower; break;
        case 'M': cmd.type = CommandType::SwitchMode; break;
        case 'N': cmd.type = CommandType::Save; break;
        case 'L': cmd.type = CommandType::Load; break;
        case 'H': cmd.type = CommandType::Help; break;
        case 'Q': cmd.type = CommandType::Quit; break;
        case 'Z':
            cmd.type = CommandType::UseSpell;
            if (!(std::cin >> cmd.argument) || cmd.argument < 1) {
                cmd.argument = -1;
            }
            break;
        default:
            cmd.type = CommandType::None;
            break;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return cmd;
}