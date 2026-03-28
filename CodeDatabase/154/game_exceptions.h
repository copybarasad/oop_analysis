#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <exception>
#include <string>
#include <sstream>

class GameException : public std::exception {
public:
    explicit GameException(const std::string& message) : message_(message) {}
    const char* what() const noexcept override {
        return message_.c_str();
    }
protected:
    std::string message_;
};

class SaveGameException : public GameException {
public:
    explicit SaveGameException(const std::string& filename, const std::string& details)
        : GameException(formatMessage("Save error", filename, details)) {}
private:
    static std::string formatMessage(const std::string& operation, const std::string& filename, const std::string& details) {
        std::ostringstream oss;
        oss << operation << " (file: " << filename << "): " << details;
        return oss.str();
    }
};

class LoadGameException : public GameException {
public:
    explicit LoadGameException(const std::string& filename, const std::string& details)
        : GameException(formatMessage("Load error", filename, details)) {}
private:
    static std::string formatMessage(const std::string& operation, const std::string& filename, const std::string& details) {
        std::ostringstream oss;
        oss << operation << " (file: " << filename << "): " << details;
        return oss.str();
    }
};

class FileAccessException : public GameException {
public:
    explicit FileAccessException(const std::string& filename, const std::string& operation)
        : GameException(formatMessage(filename, operation)) {}
private:
    static std::string formatMessage(const std::string& filename, const std::string& operation) {
        std::ostringstream oss;
        oss << "File access error (file: " << filename << "): " << operation;
        return oss.str();
    }
};

class InvalidGameStateException : public GameException {
public:
    explicit InvalidGameStateException(const std::string& details)
        : GameException(std::string("Invalid game state: ") + details) {}
};

class LevelException : public GameException {
public:
    explicit LevelException(int level, const std::string& details)
        : GameException(formatMessage(level, details)) {}
private:
    static std::string formatMessage(int level, const std::string& details) {
        std::ostringstream oss;
        oss << "Level " << level << " error: " << details;
        return oss.str();
    }
};

#endif
