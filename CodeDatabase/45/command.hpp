#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

class Command {
private:
    std::string text;
public:
    Command(std::string text);
    std::string get_text();
};

#endif