//
// Created by bob on 10/15/25.
//

#include "InputController.h"

char InputController::getCommand() const {
    std::cout << "Commands: [D]right, [A]left, [W]up, [S]down, [C]switch, [F]shoot, [Q]quit\n";
    std::cout << "Commands: [B]buy, [X]spells\n";
    std::cout << "Enter command: ";
    char op;
    std::cin >> op;
    std::cout << "\033[A" << "\r" << "\033[K";
    return tolower(op);
}

Direction InputController::getShootDirection() const {

    do{
        std::cout << "Shoot direction [W]up, [A]left, [S]down, [D]right: ";
        char dir;
        std::cin >> dir;
        switch (tolower(dir)) {
            case 'w': return Direction::Up;
            case 'a': return Direction::Left;
            case 's': return Direction::Down;
            case 'd': return Direction::Right;
        }
        std::cout << "\033[A" << "\r" << "\033[K";
    }while (true);
}

bool InputController::saveMenu() const {
    do {
        std::cout << "Do want to save this game? [y/n]: ";
        char ch;
        std::cin >> ch;
        switch (tolower(ch)) {
            case 'y': return true;
            case 'n': return false;
        }
        std::cout << "\033[A" << "\r" << "\033[K";
    }while (true);

}


