#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;
    std::string location;
    
public:
    GameException(const std::string& msg, const std::string& loc = "")
        : message(msg), location(loc) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
    
    std::string getFullMessage() const {
        if (location.empty()) {
            return message;
        }
        return "[" + location + "] " + message;
    }
};

class SaveGameException : public GameException {
public:
    SaveGameException(const std::string& msg, const std::string& loc = "")
        : GameException("Save error: " + msg, loc) {}
};

class LoadGameException : public GameException {
public:
    LoadGameException(const std::string& msg, const std::string& loc = "")
        : GameException("Load error: " + msg, loc) {}
};

class FileWriteException : public SaveGameException {
public:
    FileWriteException(const std::string& filename)
        : SaveGameException("Cannot write to file: " + filename, "FileWriteException") {}
};

class FileReadException : public LoadGameException {
public:
    FileReadException(const std::string& filename)
        : LoadGameException("Cannot read file: " + filename, "FileReadException") {}
};

class FileNotFoundException : public LoadGameException {
public:
    FileNotFoundException(const std::string& filename)
        : LoadGameException("File not found: " + filename, "FileNotFoundException") {}
};

class CorruptedDataException : public LoadGameException {
public:
    CorruptedDataException(const std::string& details)
        : LoadGameException("Corrupted save data: " + details, "CorruptedDataException") {}
};

class InvalidDataFormatException : public LoadGameException {
public:
    InvalidDataFormatException(const std::string& expected, const std::string& got)
        : LoadGameException("Invalid data format. Expected: " + expected + ", Got: " + got, 
                           "InvalidDataFormatException") {}
};

#endif
