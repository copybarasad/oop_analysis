#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include <stdexcept>
#include <string>

// Базовый класс для всех исключений игры
class GameException : public std::runtime_error {
public:
    GameException(const std::string& message, const std::string& location);
    virtual ~GameException() = default;
    
    const std::string& getLocation() const;
    
protected:
    std::string location;
};

// Исключение для ошибок сохранения файла
class FileSaveException : public GameException {
public:
    FileSaveException(const std::string& filename, const std::string& reason, const std::string& location);
    virtual ~FileSaveException() = default;
    
    const std::string& getFilename() const;
    const std::string& getReason() const;
    
private:
    std::string filename;
    std::string reason;
};

// Исключение для ошибок загрузки файла
class FileLoadException : public GameException {
public:
    FileLoadException(const std::string& filename, const std::string& reason, const std::string& location);
    virtual ~FileLoadException() = default;
    
    const std::string& getFilename() const;
    const std::string& getReason() const;
    
private:
    std::string filename;
    std::string reason;
};

// Исключение для поврежденных файлов сохранения
class CorruptedSaveFileException : public FileLoadException {
public:
    CorruptedSaveFileException(const std::string& filename, const std::string& details, const std::string& location);
    virtual ~CorruptedSaveFileException() = default;
    
    const std::string& getDetails() const;
    
private:
    std::string details;
};

// Исключение для ошибок валидации данных
class InvalidDataException : public GameException {
public:
    InvalidDataException(const std::string& dataType, const std::string& reason, const std::string& location);
    virtual ~InvalidDataException() = default;
    
    const std::string& getDataType() const;
    const std::string& getReason() const;
    
private:
    std::string dataType;
    std::string reason;
};

#endif // GAME_EXCEPTION_H

