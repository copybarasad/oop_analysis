#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <stdexcept>
#include <string>

class SaveException : public std::runtime_error {
public:
    SaveException(const std::string& message) : std::runtime_error("Ощибка сохранения: " + message) {}
};

class LoadException : public std::runtime_error {
public:
    LoadException(const std::string& message) : std::runtime_error("Ошибка загрузки: " + message) {}
};

class FileNotFoundException : public LoadException {
public:
    FileNotFoundException(const std::string& filename) 
        : LoadException("Файл не найден: " + filename) {}
};

class CorruptedSaveException : public LoadException {
public:
    CorruptedSaveException(const std::string& details) 
        : LoadException("Поврежденный файл сохранения: " + details) {}
};

class ValidationException : public std::runtime_error {
public:
    ValidationException(const std::string& message) : std::runtime_error("🔍 Ошибка валидации: " + message) {}
};

#endif