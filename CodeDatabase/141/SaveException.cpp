#include "SaveException.h"

SaveException::SaveException(const std::string &message)
    : std::runtime_error("Save error: " + message) {
}
