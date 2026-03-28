#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <exception>
#include <string>

class GameException : public std::exception {
public:
    explicit GameException(const std::string& message) : errorMessage(message) {}
    virtual ~GameException() = default;
    
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
protected:
    std::string errorMessage;
};

class SaveFileException : public GameException {
public:
    explicit SaveFileException(const std::string& filename, const std::string& reason)
        : GameException("Ошибка работы с файлом сохранения '" + filename + "': " + reason) {}
};

class SaveFileWriteException : public SaveFileException {
public:
    explicit SaveFileWriteException(const std::string& filename)
        : SaveFileException(filename, "невозможно записать в файл") {}
};

class SaveFileReadException : public SaveFileException {
public:
    explicit SaveFileReadException(const std::string& filename)
        : SaveFileException(filename, "невозможно прочитать файл") {}
};

class SaveFileNotFoundException : public SaveFileException {
public:
    explicit SaveFileNotFoundException(const std::string& filename)
        : SaveFileException(filename, "файл не существует") {}
};

class SaveFileCorruptedException : public SaveFileException {
public:
    explicit SaveFileCorruptedException(const std::string& filename, const std::string& details)
        : SaveFileException(filename, "файл содержит некорректные данные: " + details) {}
};

class GameStateException : public GameException {
public:
    explicit GameStateException(const std::string& message)
        : GameException("Ошибка состояния игры: " + message) {}
};

class InvalidLevelException : public GameException {
public:
    explicit InvalidLevelException(int levelNumber)
        : GameException("Некорректный уровень: " + std::to_string(levelNumber)) {}
};

#endif

