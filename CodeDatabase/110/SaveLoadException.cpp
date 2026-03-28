#include "SaveLoadException.hpp"

SaveLoadException::SaveLoadException (const std::string& message) :
    std::runtime_error(message) {}