#pragma once
#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;
    std::string location;

public:
    GameException(const std::string& msg, const std::string& loc = "")
        : message(msg), location(loc) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

    const std::string& getLocation() const {
        return location;
    }

    virtual std::string getFullMessage() const {
        if (location.empty()) {
            return message;
        }
        return "[" + location + "] " + message;
    }
};

class SaveException : public GameException {
public:
    SaveException(const std::string& msg, const std::string& loc = "SaveSystem")
        : GameException("Save error: " + msg, loc) {}
};

class LoadException : public GameException {
public:
    LoadException(const std::string& msg, const std::string& loc = "SaveSystem")
        : GameException("Load error: " + msg, loc) {}
};

class FileNotFoundException : public LoadException {
private:
    std::string filename;

public:
    FileNotFoundException(const std::string& file, const std::string& loc = "SaveSystem")
        : LoadException("File not found: " + file, loc), filename(file) {}

    const std::string& getFilename() const {
        return filename;
    }
};

class CorruptedDataException : public LoadException {
private:
    std::string details;

public:
    CorruptedDataException(const std::string& detail, const std::string& loc = "SaveSystem")
        : LoadException("Corrupted or invalid data: " + detail, loc), details(detail) {}

    const std::string& getDetails() const {
        return details;
    }
};

class FileAccessException : public SaveException {
private:
    std::string filename;
    std::string operation;

public:
    FileAccessException(const std::string& file, const std::string& op, const std::string& loc = "SaveSystem")
        : SaveException("Cannot " + op + " file: " + file, loc),
          filename(file), operation(op) {}

    const std::string& getFilename() const {
        return filename;
    }

    const std::string& getOperation() const {
        return operation;
    }
};
