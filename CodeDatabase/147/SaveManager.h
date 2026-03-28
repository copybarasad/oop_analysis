#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

// Базовое исключение для системы сохранений/загрузок
class SaveException : public std::runtime_error {
public:
    explicit SaveException(const std::string& msg)
        : std::runtime_error("Save/Load error: " + msg) {}
};

// Ошибка при записи в файл
class SaveWriteException : public SaveException {
public:
    explicit SaveWriteException(const std::string& msg)
        : SaveException("Write failed: " + msg) {}
};

// Ошибка при чтении файла (пригодится позже для загрузки)
class SaveReadException : public SaveException {
public:
    explicit SaveReadException(const std::string& msg)
        : SaveException("Read failed: " + msg) {}
};

// Ошибка формата файла (пригодится позже для загрузки)
class SaveFormatException : public SaveException {
public:
    explicit SaveFormatException(const std::string& msg)
        : SaveException("Invalid format: " + msg) {}
};

class SaveManager {
public:
    // Записать пару "ключ / значение" в файл:
    // key  -> отдельная строка
    // value -> следующая строка
    // При ошибке записи кидает SaveWriteException.
    static void writePair(std::ofstream& out,
                          const std::string& key,
                          const std::string& value);

    // чтение пары ключ/значение
    static bool readPair(std::ifstream& in,
                         std::string& key,
                         std::string& value);

};
