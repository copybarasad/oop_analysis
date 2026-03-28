#ifndef INPUT_MASTER_HPP
#define INPUT_MASTER_HPP

#include <string>
#include "command.hpp"

class InputMaster {
public:
    ~InputMaster() = default;
    virtual Command get_string() = 0;
    virtual Command get_char() = 0;
};

#endif