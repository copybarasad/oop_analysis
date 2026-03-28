#include <ScreenSize.hpp>
#include <ncurses.h>

ScreenSize::ScreenSize() {
    getmaxyx(stdscr, yMax, xMax);
}