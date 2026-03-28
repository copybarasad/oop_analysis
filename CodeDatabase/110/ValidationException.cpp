#include "ValidationException.hpp"

ValidationException::ValidationException (const std::string& message) :
    SaveLoadException("Validation failed: " + message) {}