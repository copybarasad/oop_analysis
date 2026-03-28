#ifndef CORRUPTEDSAVEEXCEPTION_H
#define CORRUPTEDSAVEEXCEPTION_H

#include "GameException.h"

/**
 * @brief Исключение: повреждённый файл сохранения
 */
class CorruptedSaveException : public GameException {
public:
    explicit CorruptedSaveException(const std::string& filename) 
        : GameException("Corrupted save file: " + filename + 
                       ". Data is invalid or incomplete.") {}
};

#endif // CORRUPTEDSAVEEXCEPTION_H


