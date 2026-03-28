#include "SaveLoadException.h"

SaveException::SaveException(const std::string& msg)
 : std::runtime_error(msg)
 {}


LoadException::LoadException(const std::string& msg)
    : std::runtime_error(msg) 
{}