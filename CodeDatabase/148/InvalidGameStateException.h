#ifndef INVALIDGAMESTATEEXCEPTION_H
#define INVALIDGAMESTATEEXCEPTION_H

#include "GameException.h"

/**
 * @brief Исключение: некорректное игровое состояние
 */
class InvalidGameStateException : public GameException {
public:
    explicit InvalidGameStateException(const std::string& reason) 
        : GameException("Invalid game state: " + reason) {}
};

#endif // INVALIDGAMESTATEEXCEPTION_H


