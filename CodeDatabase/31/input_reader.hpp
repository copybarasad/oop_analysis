#ifndef INPUT_READER_HPP
#define INPUT_READER_HPP

#include "command.hpp"
#include "field/point.hpp"
#include <cstdio>

class InputReader {
public:
    Command read_input();
    bool move_cursor(Command command, Point& point, int xmin, int xmax, int ymin, int ymax);
    Command get_confirm_command() const;
};

#endif