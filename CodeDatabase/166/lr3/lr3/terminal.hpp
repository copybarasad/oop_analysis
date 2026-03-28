#ifndef TERMINAL_HPP_INCLUDED
#define TERMINAL_HPP_INCLUDED

#include <termios.h>

class TerminalRaw {
public:
    void enable();
    void disable();
    ~TerminalRaw();

private:
    termios old{};
    bool active{false};
};

int read_key_immediate();
void cursor_move(int y, int x);

#endif

