#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

// Иерархия исключений игры
// Простые исключения без статических методов. 
// Каждое исключение создается напрямую с нужным сообщением.

// Базовое исключение игры
class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) 
        : std::runtime_error(message) {}
};

// Исключения файловых операций
class FileException : public GameException {
public:
    explicit FileException(const std::string& message) 
        : GameException(message) {}
};

// Исключения сохранения/загрузки
class SaveLoadException : public GameException {
public:
    explicit SaveLoadException(const std::string& message) 
        : GameException(message) {}
};

// Исключения генерации уровней
class LevelGenerationException : public GameException {
public:
    explicit LevelGenerationException(const std::string& message) 
        : GameException(message) {}
};

// Исключения заклинаний
class SpellException : public GameException {
public:
    explicit SpellException(const std::string& message) 
        : GameException(message) {}
};

// Исключения игрового поля
class FieldException : public GameException {
public:
    explicit FieldException(const std::string& message) 
        : GameException(message) {}
};

// Исключения ИИ
class AIException : public GameException {
public:
    explicit AIException(const std::string& message) 
        : GameException(message) {}
};

// Исключения игрока
class PlayerException : public GameException {
public:
    explicit PlayerException(const std::string& message) 
        : GameException(message) {}
};

// Исключения боевой системы
class CombatException : public GameException {
public:
    explicit CombatException(const std::string& message) 
        : GameException(message) {}
};

// Исключения конфигурации
class ConfigException : public GameException {
public:
    explicit ConfigException(const std::string& message) 
        : GameException(message) {}
};

#endif
