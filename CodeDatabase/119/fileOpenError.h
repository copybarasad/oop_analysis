#ifndef FILEOPENERROR_H
#define FILEOPENERROR_H
#include <stdexcept>
#include <string>
#include "gameException.h"

class FileOpenError : public GameException {
public:
    explicit FileOpenError(const std::string& filename) 
        : GameException("Ошибка: Не удалось открыть файл " + filename) {}
};

#endif