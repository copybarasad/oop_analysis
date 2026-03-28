#include "LoadException.h"

LoadException::LoadException(const std::string &message)
    : std::runtime_error("Load error: " + message) {
}
