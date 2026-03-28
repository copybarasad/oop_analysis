#pragma once
#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};

class SaveFileOpenException : public GameException {
public:
    explicit SaveFileOpenException(const std::string& filename)
        : GameException("Не удалось открыть файл для сохранения: " + filename) {}
};

class LoadFileOpenException : public GameException {
public:
    explicit LoadFileOpenException(const std::string& filename)
        : GameException("Не удалось открыть файл сохранения: " + filename) {}
};

class InvalidSaveDataException : public GameException {
public:
    explicit InvalidSaveDataException(const std::string& reason)
        : GameException("Некорректные данные сохранения: " + reason) {}
};
