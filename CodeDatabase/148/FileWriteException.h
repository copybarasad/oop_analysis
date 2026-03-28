#ifndef FILEWRITEEXCEPTION_H
#define FILEWRITEEXCEPTION_H

#include "GameException.h"

/**
 * @brief Исключение: ошибка записи в файл
 */
class FileWriteException : public GameException {
public:
    explicit FileWriteException(const std::string& filename) 
        : GameException("Failed to write to file: " + filename) {}
};

#endif // FILEWRITEEXCEPTION_H


