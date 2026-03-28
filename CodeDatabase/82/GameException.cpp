#include "GameException.h"

GameException::GameException(const std::string& message)
    : std::runtime_error(message) {}

SaveLoadException::SaveLoadException(const std::string& message)
    : GameException(message) {}