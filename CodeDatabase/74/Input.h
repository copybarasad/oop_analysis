#ifndef UNIVERSAL_INPUT_H
#define UNIVERSAL_INPUT_H

#if defined(_WIN32) || defined(_WIN64)

#include <conio.h>

inline char getInput() {

    if (_kbhit()) {
        int c = _getch();
        return static_cast<char>(c);
    }
    return 0;
}

#else // POSIX

#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>


inline char getInput() {
    char ch = 0;

    struct termios oldt;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) {
        return 0;
    }
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
#if defined(__APPLE__) || defined(__MACH__)
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
#else
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
#endif

    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) {
        return 0;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int ready = select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &tv);
    if (ready > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        if (n <= 0) ch = 0;
    }
    else {
        ch = 0;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

#endif // POSIX

#endif // UNIVERSAL_INPUT_H
