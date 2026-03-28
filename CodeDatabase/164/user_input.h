#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <iostream>
#include <sstream>
#include <string>
#include <array>

class User_input{
    public:
    User_input();

    std::array<unsigned int, 2> input_coords(unsigned int height, unsigned int width);

    std::array<unsigned int, 2> input_coords_in_range(unsigned int x_base, unsigned int y_base, 
            unsigned int height, unsigned int width, unsigned int range);

    unsigned int input_int();

    char input_char();

    void input_enter();
};

#endif