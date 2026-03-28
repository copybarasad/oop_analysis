#pragma once
#include <string>
#include <stdexcept>
#include <iostream>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) 
        : std::runtime_error(message) {}
};

class SaveLoadException : public GameException {
public:
    explicit SaveLoadException(const std::string& message) 
        : GameException("Save/Load Error: " + message) {}
};

class FileNotFoundException : public SaveLoadException {
public:
    explicit FileNotFoundException(const std::string& filename) 
        : SaveLoadException("File not found: " + filename) {}
};

class CorruptedSaveException : public SaveLoadException {
public:
    explicit CorruptedSaveException(const std::string& details) 
        : SaveLoadException("Corrupted save file: " + details) {}
};

class IOException : public SaveLoadException {
public:
    explicit IOException(const std::string& operation) 
        : SaveLoadException("I/O error during " + operation) {}
};

class ExceptionHandler {
public:
    static void handleSaveLoadError(const SaveLoadException& e);
    static void handleGameError(const GameException& e);
    static void handleGenericError(const std::exception& e);
    static bool shouldRetryOperation(const std::string& operation);
};