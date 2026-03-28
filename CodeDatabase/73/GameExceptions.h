#ifndef GAMELB1_GAMEEXCEPTIONS_H
#define GAMELB1_GAMEEXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) : std::runtime_error(message) {}
};

class SaveFileException : public GameException {
public:
    explicit SaveFileException(const std::string& message)
        : GameException("Save File Error: " + message) {}
};

class LoadFileException : public GameException {
public:
    explicit LoadFileException(const std::string& message)
        : GameException("Load File Error: " + message) {}
};

class CorruptedDataException : public GameException {
public:
    explicit CorruptedDataException(const std::string& message)
        : GameException("Data Corruption Error: " + message) {}
};

#endif //GAMELB1_GAMEEXCEPTIONS_H