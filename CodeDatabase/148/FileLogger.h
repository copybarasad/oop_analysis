#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "IGameLogger.h"
#include <fstream>
#include <string>

/**
 * @file FileLogger.h
 * @brief Логгер в файл
 */

/**
 * @class FileLogger
 * @brief Записывает события в файл
 */
class FileLogger : public IGameLogger {
private:
    std::ofstream logFile;  ///< Файловый поток
    std::string filepath;   ///< Путь к файлу
    bool isOpen;            ///< Флаг открытого файла
    
public:
    /**
     * @brief Конструктор
     * @param filename Путь к файлу лога
     */
    explicit FileLogger(const std::string& filename);
    
    /**
     * @brief Деструктор
     */
    ~FileLogger() override;
    
    /**
     * @brief Записать событие в файл
     */
    void logEvent(const GameEvent& event) override;
    
    /**
     * @brief Закрыть файл
     */
    void close() override;
    
    /**
     * @brief Проверить открыт ли файл
     */
    bool isFileOpen() const { return isOpen; }
};

#endif // FILELOGGER_H


