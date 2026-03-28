#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include "GameException.h"

/**
 * @brief Исключение: файл не найден
 */
class FileNotFoundException : public GameException {
public:
    explicit FileNotFoundException(const std::string& filename) 
        : GameException("File not found: " + filename) {}
};

#endif // FILENOTFOUNDEXCEPTION_H


