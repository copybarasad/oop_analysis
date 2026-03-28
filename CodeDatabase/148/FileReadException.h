#ifndef FILEREADEXCEPTION_H
#define FILEREADEXCEPTION_H

#include "GameException.h"

/**
 * @brief Исключение: ошибка чтения файла
 */
class FileReadException : public GameException {
public:
    explicit FileReadException(const std::string& filename, const std::string& reason = "") 
        : GameException("Failed to read file: " + filename + 
                       (reason.empty() ? "" : " (Reason: " + reason + ")")) {}
};

#endif // FILEREADEXCEPTION_H


