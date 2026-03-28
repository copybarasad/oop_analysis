#pragma once

#include <string>
#include <exception>
#include <stdexcept>

class GameException : public std::exception {
protected:
    std::string message;
    std::string method;

public:
    GameException(const std::string& msg, const std::string& meth = "")
        : message(msg), method(meth) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

    const std::string& getMethod() const {
        return method;
    }
};

class SaveException : public GameException {
public:
    SaveException(const std::string& msg, const std::string& meth = "")
        : GameException("Ошибка сохранения: " + msg, meth) {    }
};

class FileWriteException : public SaveException {
private:
    std::string fileName;

public:
    FileWriteException(const std::string& file, const std::string& meth = "")
        : SaveException("Ошибка записи файла: " + file, meth), fileName(file) {}

    const std::string& getFileName() const {
        return fileName;
    }
};

class LoadException : public GameException {
public:
    LoadException(const std::string& msg, const std::string& meth = "")
        : GameException("Ошибка загрузки: " + msg, meth) {    }
};

class FileNotFoundException : public LoadException {
public:
    FileNotFoundException(const std::string& file = "", const std::string& meth = "")
        : LoadException("Файл не найден: " + file, meth) {    }
};

class InvalidSaveDataException : public LoadException {
public:
    InvalidSaveDataException(const std::string& msg, const std::string& meth = "")
        : LoadException("Неверные данные сохранения: " + msg, meth) {}
};
