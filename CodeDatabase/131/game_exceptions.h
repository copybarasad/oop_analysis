#ifndef OOP_GAME_EXCEPTIONS_H_
#define OOP_GAME_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace rpg {

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};

class FileNotFoundException final : public GameException {
public:
    explicit FileNotFoundException(const std::string& filename)
        : GameException("Файл не найден: " + filename) {}
};

class FileSaveException final : public GameException {
public:
    explicit FileSaveException(const std::string& filename, const std::string& reason)
        : GameException("Не удалось сохранить файл '" + filename + "': " + reason) {}
};

class FileCorruptedException final : public GameException {
public:
    explicit FileCorruptedException(const std::string& filename, const std::string& details)
        : GameException("Файл поврежден '" + filename + "': " + details) {}
};

class InvalidDataException final : public GameException {
public:
    explicit InvalidDataException(const std::string& field, const std::string& details)
        : GameException("Некорректные данные в поле '" + field + "': " + details) {}
};

}

#endif

