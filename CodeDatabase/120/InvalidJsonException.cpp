#include "InvalidJsonException.h"

InvalidJsonException::InvalidJsonException(const std::string& details)
    : SaveLoadException("Invalid JSON data: " + details) {}
