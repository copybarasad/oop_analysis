#include "SaveLoadException.h"

SaveLoadException::SaveLoadException(const std::string& msg) : message(msg) {}

const char* SaveLoadException::what() const noexcept { return message.c_str(); }
