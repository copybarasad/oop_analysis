#include "InvalidSaveFileException.h"

InvalidSaveFileException::InvalidSaveFileException(const std::string &reason)
    : LoadException("Invalid save file: " + reason) {
}
