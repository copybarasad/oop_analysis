#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "command.h"
#include <map>
#include <string>

// Абстрактный интерфейс для считывания ввода
class InputReader {
public:
    virtual ~InputReader() = default;
    virtual Command readCommand() = 0;
};

#endif