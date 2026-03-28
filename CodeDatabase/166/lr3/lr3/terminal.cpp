#include "terminal.hpp"

#include <cctype>
#include <iostream>
#include <unistd.h>

void TerminalRaw::enable() {
    if (active) return;
    tcgetattr(STDIN_FILENO, &old);
    termios raw = old;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    std::cout << "\x1b[?25l\x1b[?7l\x1b[2J\x1b[H" << std::flush;
    active = true;
}

void TerminalRaw::disable() {
    if (!active) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    std::cout << "\x1b[?7h\x1b[?25h\x1b[0m\n" << std::flush;
    active = false;
}

TerminalRaw::~TerminalRaw() {
    disable();
}

int read_key_immediate() {
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) return 0;
    if (c == 0x1B) {
        unsigned char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return 27;
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return 27;
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return 'w';
                case 'B': return 's';
                case 'C': return 'd';
                case 'D': return 'a';
            }
        }
        return 0;
    }
    return std::tolower(c);
}

void cursor_move(int y, int x) {
    std::cout << "\x1b[" << y << ";" << x << "H";
}

