#include "InputSystem.h"
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

int custom_getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

InputSystem::InputSystem() = default;

char InputSystem::getInput() const {
#ifdef _WIN32
    return _getch();
#else
    return custom_getch();
#endif
}

char InputSystem::getDirection() const {
    char direction = getInput();
    std::cout << direction << std::endl;
    return direction;
}
