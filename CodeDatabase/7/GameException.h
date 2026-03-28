#pragma once
#include <stdexcept>
#include <string>

class SaveGameException : public std::runtime_error {
public:
    explicit SaveGameException(const std::string& message)
        : std::runtime_error("Ошибка сохранения игры: " + message) {}
};

class LoadGameException : public std::runtime_error {
public:
    explicit LoadGameException(const std::string& message)
        : std::runtime_error("Ошибка загрузки игры: " + message) {}
};