#pragma once
#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message) 
        : std::runtime_error(message) {}
};

class FileOpenException : public SaveLoadException {
public:
    explicit FileOpenException(const std::string& filename) 
        : SaveLoadException("Не удалось открыть файл: " + filename) {}
};

class FileReadException : public SaveLoadException {
public:
    explicit FileReadException(const std::string& filename) 
        : SaveLoadException("Ошибка чтения файла: " + filename) {}
};

class FileWriteException : public SaveLoadException {
public:
    explicit FileWriteException(const std::string& filename) 
        : SaveLoadException("Ошибка записи в файл: " + filename) {}
};

class InvalidDataException : public SaveLoadException {
public:
    explicit InvalidDataException(const std::string& details) 
        : SaveLoadException("Некорректные данные в файле сохранения: " + details) {}
};

class FileNotFoundException : public SaveLoadException {
public:
    explicit FileNotFoundException(const std::string& filename) 
        : SaveLoadException("Файл не найден: " + filename) {}
};

class DirectoryCreationException : public SaveLoadException {
public:
    explicit DirectoryCreationException(const std::string& directory) 
        : SaveLoadException("Не удалось создать директорию: " + directory) {}
};