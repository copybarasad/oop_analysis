#pragma once
#include <stdexcept>
#include <string>

struct SaveFileOpenError : std::runtime_error {
    explicit SaveFileOpenError(const std::string& msg)
        : std::runtime_error("ОШИБКА ОТКРЫТИЯ ФАЙЛА: " + msg) {}
};

struct SaveFileCorrupted : std::runtime_error {
    explicit SaveFileCorrupted(const std::string& msg)
        : std::runtime_error("ФАЙЛ СОХРАНЕНИЯ ПОВРЕЖДЁН: " + msg) {}
};

struct SaveWriteError : std::runtime_error {
    explicit SaveWriteError(const std::string& msg)
        : std::runtime_error("ОШИБКА ЗАПИСИ В ФАЙЛ: " + msg) {}
};

struct SaveReadError : std::runtime_error {
    explicit SaveReadError(const std::string& msg)
        : std::runtime_error("ОШИБКА ЧТЕНИЯ ИЗ ФАЙЛА: " + msg) {}
};