#include "GameException.h"

// GameException
GameException::GameException(const std::string& message, const std::string& location)
    : std::runtime_error("GameException at " + location + ": " + message), location(location) {
}

const std::string& GameException::getLocation() const {
    return location;
}

// FileSaveException
FileSaveException::FileSaveException(const std::string& filename, const std::string& reason, const std::string& location)
    : GameException("Failed to save file '" + filename + "': " + reason, location),
      filename(filename), reason(reason) {
}

const std::string& FileSaveException::getFilename() const {
    return filename;
}

const std::string& FileSaveException::getReason() const {
    return reason;
}

// FileLoadException
FileLoadException::FileLoadException(const std::string& filename, const std::string& reason, const std::string& location)
    : GameException("Failed to load file '" + filename + "': " + reason, location),
      filename(filename), reason(reason) {
}

const std::string& FileLoadException::getFilename() const {
    return filename;
}

const std::string& FileLoadException::getReason() const {
    return reason;
}

// CorruptedSaveFileException
CorruptedSaveFileException::CorruptedSaveFileException(const std::string& filename, const std::string& details, const std::string& location)
    : FileLoadException(filename, "Save file is corrupted or invalid: " + details, location),
      details(details) {
}

const std::string& CorruptedSaveFileException::getDetails() const {
    return details;
}

// InvalidDataException
InvalidDataException::InvalidDataException(const std::string& dataType, const std::string& reason, const std::string& location)
    : GameException("Invalid " + dataType + " data: " + reason, location),
      dataType(dataType), reason(reason) {
}

const std::string& InvalidDataException::getDataType() const {
    return dataType;
}

const std::string& InvalidDataException::getReason() const {
    return reason;
}

