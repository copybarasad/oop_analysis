#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) 
        : std::runtime_error(message) {}
};

class GameLogicException : public GameException {
public:
    explicit GameLogicException(const std::string& message) 
        : GameException("Game Logic Error: " + message) {}
};

class OutOfBoundsException : public GameLogicException {
public:
    OutOfBoundsException(int x, int y, int width, int height)
        : GameLogicException(formatMessage(x, y, width, height)) {}

private:
    static std::string formatMessage(int x, int y, int width, int height) {
        std::stringstream ss;
        ss << "Coordinates (" << x << ", " << y << ") are out of bounds. "
           << "Field dimensions are " << width << "x" << height << ".";
        return ss.str();
    }
};

class IOException : public GameException {
public:
    explicit IOException(const std::string& message) 
        : GameException("I/O Error: " + message) {}
};

class FileOpenException : public IOException {
public:
    explicit FileOpenException(const std::string& filename)
        : IOException("Cannot open file '" + filename + "' for reading or writing.") {}
};

class FileFormatException : public IOException {
public:
    FileFormatException(const std::string& filename, const std::string& details)
        : IOException("Invalid format in file '" + filename + "'. " + details) {}
};

#endif