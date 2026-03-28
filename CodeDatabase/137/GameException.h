#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};

class FileException : public GameException {
public:
    FileException(const std::string& filename, const std::string& operation)
        : GameException("File error: " + operation + " failed for file '" + filename + "'"),
          filename_(filename),
          operation_(operation) {}

    const std::string& GetFilename() const { return filename_; }
    const std::string& GetOperation() const { return operation_; }

private:
    std::string filename_;
    std::string operation_;
};

class FileOpenException : public FileException {
public:
    explicit FileOpenException(const std::string& filename)
        : FileException(filename, "open") {}
};

class FileReadException : public FileException {
public:
    FileReadException(const std::string& filename, const std::string& details = "")
        : FileException(filename, "read" + (details.empty() ? "" : " (" + details + ")")) {}
};

class FileWriteException : public FileException {
public:
    FileWriteException(const std::string& filename, const std::string& details = "")
        : FileException(filename, "write" + (details.empty() ? "" : " (" + details + ")")) {}
};

class SaveGameException : public GameException {
public:
    explicit SaveGameException(const std::string& message)
        : GameException("Save game error: " + message) {}
};

class LoadGameException : public GameException {
public:
    explicit LoadGameException(const std::string& message)
        : GameException("Load game error: " + message) {}
};

class CorruptedSaveException : public LoadGameException {
public:
    CorruptedSaveException(const std::string& section, const std::string& details)
        : LoadGameException("Corrupted save file in section '" + section + "': " + details),
          section_(section) {}

    const std::string& GetSection() const { return section_; }

private:
    std::string section_;
};

class ConfigException : public GameException {
public:
    explicit ConfigException(const std::string& message)
        : GameException("Configuration error: " + message) {}
};

class InvalidKeyBindingException : public ConfigException {
public:
    explicit InvalidKeyBindingException(const std::string& details)
        : ConfigException("Invalid key binding: " + details) {}
};

class InvalidGameStateException : public GameException {
public:
    explicit InvalidGameStateException(const std::string& message)
        : GameException("Invalid game state: " + message) {}
};

#endif // GAME_EXCEPTION_H
