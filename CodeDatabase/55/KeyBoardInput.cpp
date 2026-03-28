#include "KeyboardInput.h"
#include <iostream>
#include <string.h>


#ifdef _WIN32
    #include <windows.h>
    char getChar() {
        HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
        DWORD old;
        GetConsoleMode(h, &old);
        SetConsoleMode(h, old & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
        char ch;
        DWORD n;
        ReadConsoleA(h, &ch, 1, &n, nullptr);
        SetConsoleMode(h, old);
        return ch;
    }
#else
    #include <termios.h>
    #include <unistd.h>
    char getChar() {
        termios t;
        tcgetattr(STDIN_FILENO, &t);
        termios t_old = t;
        t.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        char ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
        return ch;
    }
#endif


bool KeyboardInput::safeInputInt(int& out) {
    if (!(std::cin >> out)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool KeyboardInput::safeInputChar(char& out) {
    if (!(std::cin >> out)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

std::optional<Command> KeyboardInput::parseHarpoon() {
    char choice;
    std::cout << "\033[34mВведите U, чтобы использовать гарпун\nВведите Q, чтобы выйти:\033[0m ";
    if (!safeInputChar(choice)) return std::nullopt;

    switch (std::tolower(choice)) {
        case 'u': {
            int x, y;
            std::cout << "\033[34mВведите координаты для атаки x y:\033[0m ";
            if (!safeInputInt(x) || !safeInputInt(y)) return std::nullopt;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            Command cmd{CommandID::RangedAttack, 0, 0, x, y};
            return cmd;
        }
        case 'q': return std::nullopt;
        default: return std::nullopt;
    }
}

std::optional<Command> KeyboardInput::parseSpell() {
    char choice;
    std::cout << "\033[34mВведите U, чтобы использовать заклинание\nВведите Q, чтобы выйти:\033[0m ";
    if (!safeInputChar(choice)) return std::nullopt;

    switch (std::tolower(choice)) {
        case 'u': {
            int idx, x, y;
            std::cout << "\033[34mВыберите заклинание и цель (индекс x y):\033[0m ";
            if (!safeInputInt(idx) || !safeInputInt(x) || !safeInputInt(y))
                return std::nullopt;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            Command cmd{CommandID::UseSpell, 0, 0, x, y, idx};
            return cmd;
        }
        case 'q': break;
        default: return std::nullopt;
    }
}

std::optional<Command> KeyboardInput::getNextCommand() {
    std::cout << "\033[34mВведите команду:\033[0m ";
    char input = getChar();

    switch (std::tolower(input)) {
        case 'w': return Command{CommandID::Move, 0, -1};
        case 's': return Command{CommandID::Move, 0, 1};
        case 'a': return Command{CommandID::Move, -1, 0};
        case 'd': return Command{CommandID::Move, 1, 0};
        case 't': return Command{CommandID::SwitchAttackType};
        case 'r': return parseHarpoon();
        case 'e': return parseSpell();
        case 'p': return Command{CommandID::Save};
        case 'l': return Command{CommandID::Load};
        case 'q': return Command{CommandID::Quit};
        default:
            std::cout << "\033[41mНеизвестная команда!\033[0m" << std::endl;
            return std::nullopt;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}