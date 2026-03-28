#ifndef DECISIONH
#define DECISIONH
#include <stddef.h>
#include <string>
class decision{
public:
    size_t getANumber();
    char getACharacter();
    std::string readLine();
};
#endif