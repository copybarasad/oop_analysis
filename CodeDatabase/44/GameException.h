#pragma once
#include <stdexcept>
#include <string>

class SaveGameException : public std::runtime_error {
public:
    SaveGameException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileOpenException : public SaveGameException {
public:
    FileOpenException(const std::string& filename)
        : SaveGameException("Не удалось открыть файл: " + filename) {}
};

class FileWriteException : public SaveGameException {
public:
    FileWriteException(const std::string& filename)
        : SaveGameException("Не удалось записать файл: " + filename) {}
};

class LoadGameException : public std::runtime_error {
public:
    LoadGameException(const std::string& msg) : std::runtime_error(msg) {}
};

class JsonParseException : public LoadGameException {
public:
    JsonParseException(const std::string& msg) : LoadGameException("Ошибка JSON: " + msg) {}
};

class UnsupportedVersionException : public LoadGameException {
public:
    UnsupportedVersionException(int version)
        : LoadGameException("Неподдерживаемая версия сохранения: " + std::to_string(version)) {}
};
