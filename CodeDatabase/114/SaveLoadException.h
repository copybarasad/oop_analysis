#ifndef SAVELOADEXCEPTION_H
#define SAVELOADEXCEPTION_H

#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message) 
        : std::runtime_error(message) {}
};

class SaveException : public SaveLoadException {
public:
    explicit SaveException(const std::string& message) 
        : SaveLoadException("Ошибка сохранения: " + message) {}
};

class LoadException : public SaveLoadException {
public:
    explicit LoadException(const std::string& message) 
        : SaveLoadException("Ошибка загрузки: " + message) {}
};

#endif

