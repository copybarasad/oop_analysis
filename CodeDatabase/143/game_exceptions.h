#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class SaveLoadException : public GameException {
protected:
    std::string filename;
public:
    SaveLoadException(const std::string& msg, const std::string& file = "")
        : GameException(msg), filename(file) {
    }
    const std::string& getFilename() const { return filename; }
};

class FileOpenException : public SaveLoadException {
public:
    FileOpenException(const std::string& file, const std::string& reason = "")
        : SaveLoadException("Cannot open file '" + file + "'" +
            (reason.empty() ? "" : ": " + reason), file) {
    }
};

class FileWriteException : public SaveLoadException {
public:
    FileWriteException(const std::string& file)
        : SaveLoadException("Write failed to '" + file + "'", file) {
    }
};

class FileReadException : public SaveLoadException {
public:
    FileReadException(const std::string& file)
        : SaveLoadException("Read failed from '" + file + "'", file) {
    }
};

class CorruptedDataException : public SaveLoadException {
public:
    CorruptedDataException(const std::string& file, const std::string& detail)
        : SaveLoadException("Corrupted save '" + file + "': " + detail, file) {
    }
};

class VersionMismatchException : public SaveLoadException {
public:
    VersionMismatchException(const std::string& file, int expected, int got)
        : SaveLoadException("Version mismatch in '" + file + "': expected " +
            std::to_string(expected) + ", got " + std::to_string(got), file) {
    }
};

class TooManyEnemiesException : public GameException {
public:
    TooManyEnemiesException(int count, int max = 5)
        : GameException("Too many enemies: " + std::to_string(count) +
            ". Maximum allowed: " + std::to_string(max)) {
    }
};

#endif


