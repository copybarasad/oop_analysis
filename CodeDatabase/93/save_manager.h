#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>
#include <exception>
#include <memory>

class SaveException : public std::exception {
protected:
    std::string message_;
public:
    explicit SaveException(const std::string& msg) : message_(msg) {}
    const char* what() const noexcept override { return message_.c_str(); }
};

class SaveWriteException : public SaveException {
public:
    explicit SaveWriteException(const std::string& filename)
        : SaveException("Failed to write save file: " + filename) {}
};

class SaveReadException : public SaveException {
public:
    explicit SaveReadException(const std::string& filename)
        : SaveException("Failed to read save file: " + filename) {}
};

class SaveDataException : public SaveException {
public:
    explicit SaveDataException(const std::string& reason)
        : SaveException("Save data error: " + reason) {}
};

class SaveManager {
public:
    static void saveGame(const class GameController& game, const std::string& filename);
    static std::unique_ptr<class GameController> loadGame(const std::string& filename);
    static bool saveExists(const std::string& filename);
};

#endif