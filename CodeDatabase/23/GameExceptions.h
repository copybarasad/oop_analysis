#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    GameException(const std::string& message) : std::runtime_error(message) {}
};

class SaveException : public GameException {
public:
    SaveException(const std::string& filename, const std::string& reason) 
        : GameException("Failed to save game to '" + filename + "': " + reason), 
          filename(filename) {}
    std::string filename;
};

class LoadException : public GameException {
public:
    LoadException(const std::string& filename, const std::string& reason) 
        : GameException("Failed to load game from '" + filename + "': " + reason), 
          filename(filename) {}
    std::string filename;
};

class FileNotFoundException : public LoadException {
public:
    FileNotFoundException(const std::string& filename) 
        : LoadException(filename, "File does not exist") {}
};

class InvalidSaveFileException : public LoadException {
public:
    InvalidSaveFileException(const std::string& filename, const std::string& reason) 
        : LoadException(filename, "Invalid save file format: " + reason) {}
};

#endif 

